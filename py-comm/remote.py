import struct
import array
import serial
import time
import sys
import pygame

'''
float 		leftPwm, rightPwm;
uint8_t 	cmdType;
uint8_t 	checksum;
'''
StartByte = chr(2)
EndByte = chr(10)

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

def pwmPacket(left, right):
	floatVals = array.array('f')
	floatVals.append(left)
	floatVals.append(right)
	dataStr = floatHexStr(floatVals[0]) + floatHexStr(floatVals[1]) \
		+ byteHexStr(byteVals[0]) + byteHexStr(byteVals[1])
	return StartByte + dataStr + EndByte

try:
	sercom = serial.Serial('/COM3')
except:
	print 'Serial open failed'

pygame.init()
screen = pygame.display.set_mode((10,10))

key_pwm = {pygame.K_UP: (0.5, 0.5),
		pygame.K_LEFT: (0.0, 0.5),
		pygame.K_RIGHT: (0.5, 0.0)
		}

while True:
	events = pygame.event.get()
	pwm = None
	for event in events:
		if event.type == pygame.KEYDOWN:
			if event.key in key_pwm:
				pwm = key_pwm[event.key]
		if event.type == pygame.KEYUP:
			pwm = (0,0)
	if pwm:
		print pwm
		packet = pwmPacket(pwm[0], pwm[1])
		print packet

		try:
			sercom.write(packet)
		except:
			print 'Packet send failed'


