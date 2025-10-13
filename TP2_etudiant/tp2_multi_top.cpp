#include <limits>
#include <systemc>

#include "mapping_table.h"
#include "vci_gcd_coprocessor.h"
#include "vci_gcd_master.h"
#include "vci_param.h"
#include "vci_signals.h"
#include "vci_vgsb.h" // VCI VGSB bus

/*
- Define base address of each component
- Define segment sizes of the components
- Define VCI parameters
- Add segments to the mapping table
- Instantiate the different components (initiators, targets, bus)
- Create VCI signal bundles for each component
- Connect components to the bus via the VCI signals (netlist)
*/

// Base address and size of the GCD memory-mapped peripheral
// MSB should be different for each peripheral so that the bus can decode the address of the target
#define GCD0_BASE 0x00000000
#define GCD1_BASE 0x10000000
#define GCD2_BASE 0x20000000

// The GCD component contains 4 registers of 4 bytes each = 16 bytes or 0x10
#define GCD_SIZE (1 << 4)

int sc_main(int argc, char* argv[])
{
    using namespace sc_core;
    using namespace soclib::caba;
    using namespace soclib::common;

    // VCI fields width definition
    //	cell_size	= 4;
    // 	plen_size	= 8;
    // 	addr_size	= 32;
    // 	rerror_size	= 1;
    // 	clen_size	= 1;
    // 	rflag_size	= 1;
    // 	srcid_size	= 12;
    // 	trdid_size	= 1;
    // 	pktid_size	= 1;
    // 	wrplen_size	= 1;

    typedef VciParams<4, 8, 32, 1, 1, 1, 12, 1, 1, 1> vci_param;

    ///////////////////////////////////////////////////////////////////////////
    // simulation arguments : number of cycles & seed for the random generation
    ///////////////////////////////////////////////////////////////////////////
    int ncycles = std::numeric_limits<int>::max();
    int seed    = 123456789;
    if (argc > 1)
        ncycles = atoi(argv[1]);
    if (argc > 2)
        seed = atoi(argv[2]);

    //////////////////////////////////////////////////////////////////////////
    // Mapping Table
    //////////////////////////////////////////////////////////////////////////
    // We are using only 1 level of indexing for both initiators and targets
    MappingTable maptab(32, IntTab(8), IntTab(8), 0x03000000);
    maptab.add(soclib::common::Segment("GCD0", GCD0_BASE, GCD_SIZE, IntTab(0), true));
    maptab.add(soclib::common::Segment("GCD1", GCD1_BASE, GCD_SIZE, IntTab(1), true));
    maptab.add(soclib::common::Segment("GCD2", GCD2_BASE, GCD_SIZE, IntTab(2), true));
    std::cout << std::endl << maptab << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // Signals
    //////////////////////////////////////////////////////////////////////////
    sc_clock signal_clk("signal_clk", sc_time(1, SC_NS), 0.5);
    sc_signal<bool> signal_resetn("signal_resetn");
    // Since we are now using multiple initiators and multiple targets, we need to define multiple signals
    // Initiators/Masters
    VciSignals<vci_param> signal_vci_m0("signal_vci_m0");
    VciSignals<vci_param> signal_vci_m1("signal_vci_m1");
    VciSignals<vci_param> signal_vci_m2("signal_vci_m2");
    // Targets/Slaves
    VciSignals<vci_param> signal_vci_s0("signal_vci_s0");
    VciSignals<vci_param> signal_vci_s1("signal_vci_s1");
    VciSignals<vci_param> signal_vci_s2("signal_vci_s2");

    //////////////////////////////////////////////////////////////////////////
    // Components
    //////////////////////////////////////////////////////////////////////////
    /* Note: Dans ce TP, on utilisera *un seul niveau d'indexation* (cf. int_tab.h),
    alors que SoCLib utilise des index composites. */

    // Initiators/Masters
    VciGcdMaster<vci_param> master_0("gcd_master_0", IntTab(0), maptab, seed, GCD0_BASE);
    VciGcdMaster<vci_param> master_1("gcd_master_1", IntTab(1), maptab, seed, GCD1_BASE);
    VciGcdMaster<vci_param> master_2("gcd_master_2", IntTab(2), maptab, seed, GCD2_BASE);
    // Targets/Slaves
    VciGcdCoprocessor<vci_param> coproc_0("gcd_coprocessor_0", IntTab(0), maptab);
    VciGcdCoprocessor<vci_param> coproc_1("gcd_coprocessor_1", IntTab(1), maptab);
    VciGcdCoprocessor<vci_param> coproc_2("gcd_coprocessor_2", IntTab(2), maptab);
    // VCI VGSB bus (3 initiators, 3 targets)
    VciVgsb<vci_param> vci_bus("vci_vgsb", maptab, 3, 3);

    //////////////////////////////////////////////////////////////////////////
    // Net-List
    //////////////////////////////////////////////////////////////////////////
    // Master 0
    master_0.p_clk(signal_clk);
    master_0.p_resetn(signal_resetn);
    master_0.p_vci(signal_vci_m0);
    // Master 1
    master_1.p_clk(signal_clk);
    master_1.p_resetn(signal_resetn);
    master_1.p_vci(signal_vci_m1);
    // Master 2
    master_2.p_clk(signal_clk);
    master_2.p_resetn(signal_resetn);
    master_2.p_vci(signal_vci_m2);
    // Coprocessor 0
    coproc_0.p_clk(signal_clk);
    coproc_0.p_resetn(signal_resetn);
    coproc_0.p_vci(signal_vci_s0);
    // Coprocessor 1
    coproc_1.p_clk(signal_clk);
    coproc_1.p_resetn(signal_resetn);
    coproc_1.p_vci(signal_vci_s1);
    // Coprocessor 2
    coproc_2.p_clk(signal_clk);
    coproc_2.p_resetn(signal_resetn);
    coproc_2.p_vci(signal_vci_s2);
    // VCI VGSB bus
    vci_bus.p_clk(signal_clk);
    vci_bus.p_resetn(signal_resetn);
    // Connect initiators/masters to the bus
    vci_bus.p_to_initiator[0](signal_vci_m0);
    vci_bus.p_to_initiator[1](signal_vci_m1);
    vci_bus.p_to_initiator[2](signal_vci_m2);
    // Connect targets/slaves to the bus
    vci_bus.p_to_target[0](signal_vci_s0);
    vci_bus.p_to_target[1](signal_vci_s1);
    vci_bus.p_to_target[2](signal_vci_s2);

    //////////////////////////////////////////////////////////////////////////
    // simulation
    //////////////////////////////////////////////////////////////////////////
    signal_resetn = false;
    sc_start(sc_time(1, SC_NS));

    signal_resetn = true;
    for (size_t n = 1; n < ncycles; n++)
        sc_start(sc_time(1, SC_NS));

    return (0);

} // end sc_main
