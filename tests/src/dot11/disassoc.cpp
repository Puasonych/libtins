#include <gtest/gtest.h>
#include <algorithm>
#include <memory>
#include <stdint.h>
#include "dot11.h"
#include "tests/dot11.h"


using namespace std;
using namespace Tins;

typedef Dot11::address_type address_type;

class Dot11DisassocTest : public testing::Test {
public:
    static const address_type empty_addr, hwaddr;
    static const uint8_t expected_packet[];
    //static void test_equals_expected(const Dot11Beacon&dot11);
};

const uint8_t Dot11DisassocTest::expected_packet[] = { 
    '\xa1', '\x01', 'O', '#', '\x00', '\x01', '\x02', '\x03', '\x04', 
    '\x05', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x02', 
    '\x03', '\x04', '\x05', '\x06', '\x07', '\x00', '\x00', '\x12', '#'
};

void test_equals(const Dot11Disassoc &dot1, const Dot11Disassoc &dot2) {
    EXPECT_EQ(dot1.reason_code(), dot2.reason_code());
    test_equals(
        static_cast<const Dot11ManagementFrame&>(dot1),
        static_cast<const Dot11ManagementFrame&>(dot2)
    );
}

void test_equals_expected(const Dot11Disassoc &dot11) {
    test_equals_expected(static_cast<const Dot11ManagementFrame&>(dot11));
    EXPECT_EQ(dot11.reason_code(), 0x2312);
    EXPECT_EQ(dot11.subtype(), Dot11::DISASSOC);
}

TEST_F(Dot11DisassocTest, Constructor) {
    Dot11Disassoc dot11;
    test_equals_empty(static_cast<const Dot11ManagementFrame&>(dot11));
    EXPECT_EQ(dot11.reason_code(), 0);
}

TEST_F(Dot11DisassocTest, ConstructorFromBuffer) {
    Dot11Disassoc dot11(expected_packet, sizeof(expected_packet));
    test_equals_expected(dot11);
}

TEST_F(Dot11DisassocTest, CopyConstructor) {
    Dot11Disassoc dot1(expected_packet, sizeof(expected_packet));
    Dot11Disassoc dot2(dot1);
    test_equals(dot1, dot2);
}

TEST_F(Dot11DisassocTest, CopyAssignmentOperator) {
    Dot11Disassoc dot1(expected_packet, sizeof(expected_packet));
    Dot11Disassoc dot2;
    dot2 = dot1;
    test_equals(dot1, dot2);
}

TEST_F(Dot11DisassocTest, ClonePDU) {
    Dot11Disassoc dot1(expected_packet, sizeof(expected_packet));
    std::auto_ptr<Dot11Disassoc> dot2(dot1.clone_pdu());
    test_equals(dot1, *dot2);
}

TEST_F(Dot11DisassocTest, FromBytes) {
    std::auto_ptr<PDU> dot11(Dot11::from_bytes(expected_packet, sizeof(expected_packet)));
    ASSERT_TRUE(dot11.get());
    std::cout << (int)dot11->pdu_type() << std::endl;
    const Dot11Disassoc *disassoc = dot11->find_inner_pdu<Dot11Disassoc>();
    ASSERT_TRUE(disassoc);
    test_equals_expected(*disassoc);
}
