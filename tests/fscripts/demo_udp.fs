//
// start udp echo server first
// ./bin/udp_recv 4000
//

var a = iostream_udp_create("localhost", 4000)
var b = iostream_get_ostream(a)
var c = iostream_get_istream(a)

assert(ostream_write_uint8(b, 1) == 1)
assert(ostream_write_uint16(b, 2)== 2)
assert(ostream_write_uint32(b, 3) == 4)
assert(ostream_write_uint64(b, 4) == 8)
assert(ostream_write_float(b, 5) == 4)
assert(ostream_write_double(b, 6) == 8)
assert(ostream_write_string(b, "hello\n") == 6)

assert(istream_read_uint8(c) == 1)
assert(istream_read_uint16(c) == 2)
assert(istream_read_uint32(c) == 3)
assert(istream_read_uint64(c) == 4)
assert(istream_read_float(c) == 5)
assert(istream_read_double(c) == 6)
assert(istream_read_string(c, 6) == "hello\n")

