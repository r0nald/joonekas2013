import struct
import array
import serial
import time
import sys
import curses
from HexStrDataParser import HexStrDataParser, DataToHexStr
import datetime
import subprocess
import curses.textpad as textpad

StartByte = chr(2)
EndByte = chr(10)

try:
	sercom = serial.Serial(sys.argv[1])
except Exception as e:
	print 'Serial open failed'
	print e
	exit()


stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(1)
stdscr.nodelay(1)

currentDataWin = curses.newwin(3, 80, 5, 0)
currentDataWin.nodelay(1)

PIDconfWin = curses.newwin(5, 80, 10, 0)

'''
uint32_t 	time;					# 0	
uint32_t	lineSensors;
uint32_t	usedSensors;
uint32_t	adc1;	

float			pidFeedback;			# 4
float			pidK, pidI, pidD, pidU;		# 5 6 7 8
float			pwmLeft, pwmRight;		# 9 10

float			battVoltage;			# 11
uint8_t			finishLineDetected;		# 12
'''

parser = HexStrDataParser('IIII' + 'f' + 'ffff' + 'fff' + 'b')

'''
float 		leftPwm, rightPwm;		# 0 1
float		basePwm, pidK, pidI, pidD;	# 2 3 4 5
uint32_t 	cmdType;			# 6
enum CommandFromPC { Stop = 0, Run = 1, SetPid = 2, SetPwm  = 3 };
'''

packetBuilder = DataToHexStr('ff' + 'ffff' + 'I')
packetStruct = [0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0]

def sendPacketStruct(packetStruct):
	global sercom
	packet = StartByte + packetBuilder.dataToString(packetStruct) + EndByte
#print packet
	sercom.write(packet)

def sendStop():
	global packetStruct
	packetStruct[6] = 0
	sendPacketStruct(packetStruct)

def sendStart():
	global packetStruct
	packetStruct[6] = 1
	sendPacketStruct(packetStruct)

def sendPwm():
	global packetStruct
	packetStruct[6] = 3
	sendPacketStruct(packetStruct)

def sendControllerConf(conf):
	global packetStruct
	packetStruct[2:6] = conf
	packetStruct[6] = 2
	sendPacketStruct(packetStruct)

firstLine = False
running = False
logFile = None
fname = None
firstTime = None
conf = None
lastFinishTime = None
prevFinishLine = 0

while True:
	sercom.readline()
	line = sercom.next()

	if not firstLine:
		firstLine = True
		continue

	dataOk = True

	if line[0] != chr(0x02) or line[-1] != chr(0x0A):
		dataOk = False
		break
	try:
	 	if dataOk:
			data = parser.stringToData(line[1:-1])
	except Exception as e:
		dataOk = False

	currentDataWin.clear()

	if dataOk:
		timeTick = data[0]
		lineSensors = data[1]
		usedSensors = data[2]
		pidFeedback = data[4]
		leftPwm = data[9]
		rightPwm = data[10]
		pidU = data[8]
		pidP = data[5]
		pidI = data[6]
		pidD = data[7]
		finishLine = data[8]

		if not firstTime:
			firstTime = timeTick

		time = 0.01*(timeTick - firstTime)
	
		debugLine = '{0:0b}'.format(lineSensors).zfill(10) + ' ' +  '{0:0b}'.format(usedSensors).zfill(10) + ' ' + str(pidFeedback) + ' ' + str(pidU)

		if logFile:
			logLine = '{} {} {} {} {} {} {} {}'.format(time, pidFeedback, pidU, leftPwm, rightPwm, pidP, pidI, pidD)
			logFile.write(logLine +'\n')

		currentDataWin.addstr(0, 0, debugLine)
		currentDataWin.addstr(1, 0, str(leftPwm) + ' ' + str(rightPwm))

		if finishLine == 1:
			print 'FINISH'
			break

		if finishLine == 1 and prevFinishLine == 0:
			if lastFinishTime:
				now = datetime.datetime.now()
				timer = (now - lastFinishTime).total_seconds()
				currentDataWin.addstr(2, 0, 'Lap time : ' + str(timer))
			lastFinishTime = now
			currentDataWin.addstr(2, 0, 'Finish')
		prevFinishLine = finishLine

	else:
		currentDataWin.addstr(0, 0, 'No comm')
	
	currentDataWin.refresh()

	c = stdscr.getch()

	if c == ord('q'):
		break
	elif c == ord(' '):
		if running:
			sendStop()
			running = False
			stdscr.addstr(0, 0, 'STOPPED')
			logFile.close()
			logFile = None
			subprocess.Popen('./plot_control1 ' + fname, shell=True)
		else:
			fname = datetime.datetime.now().strftime('run%d%H%M%S.log')
			lastFinishTime = None
			logFile = open(fname, 'w')
			sendStart()
			running = True
			stdscr.addstr(0, 0, 'RUNNING')

	elif c == ord('p'):
		sendPwm()
		stdscr.addstr(0, 0, 'PWM  ')
		running = True

	elif c == ord('c'):
		tp = textpad.Textbox(PIDconfWin)
		paramStr = tp.edit()
		conf = [float(fstr) for fstr in paramStr.rsplit()]
		if len(conf) != 4:
			PIDconfWin.addstr(0, 0, 'Invalid params')
		sendControllerConf(conf)


curses.nocbreak(); stdscr.keypad(0); curses.echo()
