#include <systemc>
#include <limits>

#include "vci_gcd_master.h"
#include "vci_gcd_coprocessor.h"
#include "vci_vgsb.h"
#include "vci_signals.h"
#include "vci_param.h"
#include "mapping_table.h"

#define GCD_BASE0	0x50000000
#define GCD_SIZE	1 << 4

#define GCD_BASE1   0x60000000
#define GCD_BASE2   0x70000000

	
int sc_main(int argc, char *argv[])
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

	typedef VciParams<4,4,32,1,1,1,12,1,1,1> vci_param;

	///////////////////////////////////////////////////////////////////////////
	// simulation arguments : number of cycles & seed for the random generation
	///////////////////////////////////////////////////////////////////////////
        int ncycles = std::numeric_limits<int>::max();
        int seed    = 123456789;
        if (argc > 1) ncycles = atoi(argv[1]) ;
        if (argc > 2) seed = atoi(argv[2]) ;

	//////////////////////////////////////////////////////////////////////////
	// Mapping Table
	//////////////////////////////////////////////////////////////////////////
	MappingTable maptab(32, IntTab(8), IntTab(8), 0x03000000);
	maptab.add(soclib::common::Segment("GCD0", GCD_BASE0, GCD_SIZE, IntTab(0), true));
	maptab.add(soclib::common::Segment("GCD1", GCD_BASE1, GCD_SIZE, IntTab(1), true));
	maptab.add(soclib::common::Segment("GCD2", GCD_BASE2, GCD_SIZE, IntTab(2), true));
	std::cout << std::endl << maptab << std::endl;

	//////////////////////////////////////////////////////////////////////////
        // Signals
	//////////////////////////////////////////////////////////////////////////
        sc_clock               		signal_clk("signal_clk", sc_time( 1, SC_NS ), 0.5 );
        sc_signal<bool> 		signal_resetn("signal_resetn");
        VciSignals<vci_param> 		signal_vci_m0("signal_vci_m0");
		VciSignals<vci_param> 		signal_vci_m1("signal_vci_m1");
		VciSignals<vci_param> 		signal_vci_m2("signal_vci_m2");
		VciSignals<vci_param> 		signal_vci_s0("signal_vci_s0");
		VciSignals<vci_param> 		signal_vci_s1("signal_vci_s1");
		VciSignals<vci_param> 		signal_vci_s2("signal_vci_s2");


	//////////////////////////////////////////////////////////////////////////
	// Components
	//////////////////////////////////////////////////////////////////////////
    VciGcdMaster<vci_param> 	master_0("gcd_master0", IntTab(0), maptab, seed, GCD_BASE0);
	VciGcdMaster<vci_param> 	master_1("gcd_master1", IntTab(1), maptab, seed, GCD_BASE1);
	VciGcdMaster<vci_param> 	master_2("gcd_master2", IntTab(2), maptab, seed, GCD_BASE2);
	VciGcdCoprocessor<vci_param>	coproc_0("gcd_coprocessor0", IntTab(0), maptab);
	VciGcdCoprocessor<vci_param>	coproc_1("gcd_coprocessor1", IntTab(1), maptab);
	VciGcdCoprocessor<vci_param>	coproc_2("gcd_coprocessor2", IntTab(2), maptab);

	VciVgsb<vci_param> vci_bus("vci_vgsb", maptab, 3, 3);

	//////////////////////////////////////////////////////////////////////////
	// Net-List
	//////////////////////////////////////////////////////////////////////////
	master_0.p_clk(signal_clk);
	master_0.p_resetn(signal_resetn);
	master_0.p_vci(signal_vci_m0);
	master_1.p_clk(signal_clk);
	master_1.p_resetn(signal_resetn);
	master_1.p_vci(signal_vci_m1);
	master_2.p_clk(signal_clk);
	master_2.p_resetn(signal_resetn);
	master_2.p_vci(signal_vci_m2);

	coproc_0.p_clk(signal_clk);
	coproc_0.p_resetn(signal_resetn);
	coproc_0.p_vci(signal_vci_s0);
	coproc_1.p_clk(signal_clk);
	coproc_1.p_resetn(signal_resetn);
	coproc_1.p_vci(signal_vci_s1);
	coproc_2.p_clk(signal_clk);
	coproc_2.p_resetn(signal_resetn);
	coproc_2.p_vci(signal_vci_s2);

	vci_bus.p_clk(signal_clk);
	vci_bus.p_resetn(signal_resetn);
	vci_bus.p_to_initiator[0](signal_vci_m0);
	vci_bus.p_to_initiator[1](signal_vci_m1);
	vci_bus.p_to_initiator[2](signal_vci_m2);
	vci_bus.p_to_target[0](signal_vci_s0);
	vci_bus.p_to_target[1](signal_vci_s1);
	vci_bus.p_to_target[2](signal_vci_s2);


	//////////////////////////////////////////////////////////////////////////
	// simulation
	//////////////////////////////////////////////////////////////////////////
        signal_resetn = false;
        sc_start( sc_time( 1, SC_NS ) );

        signal_resetn = true;
        for ( size_t n=1 ; n<ncycles ; n++) sc_start( sc_time( 1, SC_NS ) ) ;

        return(0);

} // end sc_main

