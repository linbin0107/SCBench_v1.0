####################################################
##  Description: top level Makefile
##  Author: Bin Lin
##  Affiliation: SVL Lab, Portland State University
##  Date: 09/01/2016
####################################################


SUBDIRS = adpcm aes128 amba_AHB ann crossroad des disparity \
          fft fir ia-32 idct interpolation kasumi md5c mips \
          noc philosophers pkt_switch producerConsumer qsort \
          risc_cpu risc_cpu_control risc_cpu_exec risc_cpu_mmxu \
          rsa asr_abs master_slave_bus simpleRing snow3G sobel \
          tlm_b_transport tlm_dmi_dbg tlm_nb_transport uart \
          tlm_routing usbTxWireArbiter vga Y86

CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS) clean $(CLEARNDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean: $(CLEANDIRS)

$(CLEANDIRS):
	$(MAKE) -C $(@:clean-%=%) clean
