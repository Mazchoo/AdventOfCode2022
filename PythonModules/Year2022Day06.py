
import build.Release.AdventModule as m

packets = m.SignalPacket('data/2022Day06/puzzle_input.txt')
print(packets.get_packet_starts(4))
print(packets.get_packet_starts(14))
