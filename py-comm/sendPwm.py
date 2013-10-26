import struct
import array
import serial
import time
import sys

'''
float 		leftPwm, rightPwm;
uint8_t 	cmdType;
uint8_t 	checksum;
'''
StartByte = chr(2)
EndByte = chr(10)

floatVals = array.array('f')
floatVals.append(float(sys.argv[1]))
floatVals.append(float(sys.argv[2]))

byteVals = array.array('c')
byteVals.append(chr(1))
byteVals.append(chr(0))

def floatHexStr(val):
	packed = struct.pack('f', val)
	hStr = ''
	for c in packed:
		hStr = hStr + '%02x' % ord(c)
	return hStr

def byteHexStr(val):
	packed = struct.pack('c', val)
	return '%02x' % ord(packed[0])

dataStr = floatHexStr(floatVals[0]) + floatHexStr(floatVals[1]) \
	+ byteHexStr(byteVals[0]) + byteHexStr(byteVals[1])

packet = StartByte + dataStr + EndByte
print packet

try:
	sercom = serial.Serial('/dev/ttyACM0')
	print sercom.read(100)
	sercom.write(packet)
except:
	print 'Serial comm failed'




