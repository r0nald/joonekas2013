import string
import struct
import binascii
import ctypes

class HexStrDataParser:
	def __init__(self, dataFields):
		self.dataFields = dataFields
		self.packetSize = struct.calcsize(dataFields)

	def stringToBytes(self, dataString):
		if not len(dataString) % 2 == 0:
			raise Exception('len(dataString) % 2 != 0 !!! \
					invalid dataString')

		byteStream = ''
		for i in range(len(dataString)/2):
			byteStream = byteStream + chr(
					string.atoi(dataString[2*i:(2*i+2)], 16))

		return byteStream

	def stringToData(self, dataString):
		byteStream = self.stringToBytes(dataString)

		if len(byteStream) != self.packetSize:
			raise Exception('len(byteStream) != self.packetSize ' +
					str(len(byteStream)) + ' != ' + 
					str(self.packetSize) +
					' dataString doesnt match structure size')
		
		vals = struct.unpack(self.dataFields, byteStream)
		return vals

class DataToHexStr:
	def __init__(self, dataFields):
		self.dataFields = dataFields

	def dataToString(self, data):
		if len(data) != len(self.dataFields):
			raise Exception('Elements of values in data list \
					!= length of dataFields string')
		buf = ctypes.create_string_buffer(
				struct.calcsize(self.dataFields))

		offset = 0
		for f in range(len(self.dataFields)):
			struct.pack_into(self.dataFields[f], buf, 
					offset, data[f])
			offset = offset + struct.calcsize(self.dataFields[f])

		return binascii.hexlify(buf)

