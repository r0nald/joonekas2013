from HexStrDataParser import HexStrDataParser, DataToHexStr

parser = HexStrDataParser('b')
assert(parser.stringToData('0A')[0] == 10)

parser = HexStrDataParser('I')
assert(parser.stringToData('0A000000')[0] == 10)

parser = HexStrDataParser('I')
assert(parser.stringToData('00000080')[0] == 2**31)

parser = HexStrDataParser('I')
assert(parser.stringToData('00010000')[0] == 256)

parser = HexStrDataParser('Ib')
assert(parser.stringToData('000100000A')[0] == 256)
assert(parser.stringToData('000100000A')[1] == 10)

parser = HexStrDataParser('fb')
assert(parser.stringToData('0000803f0A')[0] == 1.0)
assert(parser.stringToData('0000803f0A')[1] == 10)

parser = HexStrDataParser('ff')
assert(parser.stringToData('0000803f000080bf')[0] == 1.0)
assert(parser.stringToData('0000803f000080bf')[1] == -1.0)

packer = DataToHexStr('ffB')
packedStr = packer.dataToString([1.0, -1.0, 255])

parser = HexStrDataParser('ffB')
assert(parser.stringToData(packedStr)[0] == 1.0)
assert(parser.stringToData(packedStr)[1] == -1.0)
assert(parser.stringToData(packedStr)[2] == 255)

parser = HexStrDataParser('IIII' + 'f' + 'ffff' + 'fff' + 'b')
packedStr = 'a70007000033020000300000003a0000000000000000000000355700083030000000000000f00000000000000000000001'
print parser.stringToData(packedStr)[12]
