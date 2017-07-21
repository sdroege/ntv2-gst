/**
	@file		ntv2_2022_registers.h
	@brief		Defines the Sarek board's registers.
	@copyright	(C) 2014-2017 AJA Video Systems, Inc.	Proprietary and confidential information.
**/

#ifndef REGISTERS_MB_H
#define REGISTERS_MB_H

#define SAREK_MAX_CHANS 4
#define SAREK_MAX_PORTS 2

/////////////////////////////////////////////////////////////////////
//
// General memory map definitions
//
/////////////////////////////////////////////////////////////////////

#define SAREK_REGS                  (0x100000/4)        // Base address of Sarek General Regs (expressed as regNum)
#define SAREK_GENLOCK_SPI           (0x101000/4)
#define SAREK_MAILBOX               (0x107000/4)
#define SAREK_PLL                   (0x108000/4)

#define SAREK_REGS2                 (0x10e000/4)

#define SAREK_10G_EMAC_0            (0x180000/4)
#define SAREK_10G_EMAC_1            (0x1a0000/4)

/////////////////////////////////////////////////////////////////////
//
// 2022 Sarek General Registers (expressed as regNum)
//
/////////////////////////////////////////////////////////////////////

#define kRegSarekControl            0
#define kRegSarekPma                1
#define kRegSarekSpiSelect          2
#define kRegSarekRxReset            3
#define kRegSarekMBUptime           4
#define kRegSarekMBState            5
#define kRegSarekMBBuildNum         6
#define kRegSarekSerialLow          7
#define kRegSarekSerialHi           8
#define kRegSarekIGMPVersion        9

#define kRegSarekMiscState          10
#define kRegSarekTxAutoMAC          11
#define kRegSarekunused12           12
#define kRegSarekunused13           13
#define kRegSarekunused14           14
#define kRegSarekunused15           15

#define kRegSarekMAC                16
#define kRegSarekMAC1               17
#define kRegSarekMAC2               18
#define kRegSarekMAC3               19
#define kRegSarekIP0                20
#define kRegSarekIP1                21
#define kRegSarekNET0               22
#define kRegSarekNET1               23
#define kRegSarekGATE0              24
#define kRegSarekGATE1              25
#define kRegSarekunused26           26
#define kRegSarekunused27           27
#define kRegSarekunused28           28
#define kRegSarekunused29           29
#define kSarekRegIGMPDisable        30
#define kSarekRegIGMPDisable2       31
#define kRegSarekFwCfg              32
#define kRegSarekDDRStatus          33
#define kRegSarekXrefStatus         34

#define kRegSarekDNALow             36
#define kRegSarekDNAHi              37
#define kRegSarekLicenseStatus      38

#define kRegSarekRx1Drops           40
#define kRegSarekRx2Drops           41
#define kRegSarekRx3Drops           42
#define kRegSarekRx4Drops           43

// second block
#define TS_BLOCK_BASE               0
#define IGMP_BLOCK_BASE             100
#define ENCODER_BLOCK_BASE          200
#define UNUSED_BLOCK_BASE           300

#define kRegSarekMBSeqNum           0
#define kRegSarekActProgramNum      1
#define kRegSarekActProgramPID      2
#define kRegSarekActVideoPID        3
#define kRegSarekActAudioPID        4
#define kRegSraekActPCRPID          5
#define kRegSarekAudioNumChans      6
#define kRegSarekAudioBitDepth      7
#define kRegSarekVideoBitrate       8
#define kRegSarekAudioBitrate       9

#define kRegSarekHostSeqNum         12    // set by host
#define kRegSarekModeSelect         13    // set by host
#define kRegSarekProgNumSelect      14    // set by host
#define kRegSarekProgPIDSelect      15    // set by host
#define kRegSarekAudioNumSelect     16    // set by host

#define kRegSarekNumPGMs            20
#define kRegSarekNumAudios          21

#define kRegSarekPGMNums            24
#define kRegSarekPGMPIDs            40
#define kRegSarekAudioPIDs          56

// Encoder block regs
#define kRegSarekEncodeVideoFormat1     (ENCODER_BLOCK_BASE+0)
#define kRegSarekEncodeUllMode1         (ENCODER_BLOCK_BASE+1)
#define kRegSarekEncodeBitDepth1        (ENCODER_BLOCK_BASE+2)
#define kRegSarekEncodeChromaSubSamp1   (ENCODER_BLOCK_BASE+3)
#define kRegSarekEncodeMbps1            (ENCODER_BLOCK_BASE+4)
#define kRegSarekEncodeStreamType1      (ENCODER_BLOCK_BASE+5)
#define kRegSarekEncodeProgramPid1      (ENCODER_BLOCK_BASE+6)
#define kRegSarekEncodeVideoPid1        (ENCODER_BLOCK_BASE+7)
#define kRegSarekEncodePcrPid1          (ENCODER_BLOCK_BASE+8)
#define kRegSarekEncodeAudio1Pid1       (ENCODER_BLOCK_BASE+9)

#define kRegSarekEncodeVideoFormat2     (ENCODER_BLOCK_BASE+10)
#define kRegSarekEncodeUllMode2         (ENCODER_BLOCK_BASE+11)
#define kRegSarekEncodeBitDepth2        (ENCODER_BLOCK_BASE+12)
#define kRegSarekEncodeChromaSubSamp2   (ENCODER_BLOCK_BASE+13)
#define kRegSarekEncodeMbps2            (ENCODER_BLOCK_BASE+14)
#define kRegSarekEncodeStreamType2      (ENCODER_BLOCK_BASE+15)
#define kRegSarekEncodeProgramPid2      (ENCODER_BLOCK_BASE+16)
#define kRegSarekEncodeVideoPid2        (ENCODER_BLOCK_BASE+17)
#define kRegSarekEncodePcrPid2          (ENCODER_BLOCK_BASE+18)
#define kRegSarekEncodeAudio1Pid2       (ENCODER_BLOCK_BASE+19)


/////////////////////////////////////////////////////////////////////
//
// 2022 Sarek General Registers Bit Definitions
//
/////////////////////////////////////////////////////////////////////

#define RESET_MB                    BIT(0)
#define VIRTUAL_UART                BIT(4)
#define DECODER_1_RESET             BIT(11)
#define ENCODER_1_ENABLE            BIT(16)
#define ENCODER_1_MD_ENABLE         BIT(17)
#define ENCODER_1_SF_ENABLE         BIT(18)
#define DECODER_1_NON_ELSM          BIT(19)
#define ENCODER_1_CFG_NUM           (BIT(20) + BIT(21) + BIT(22))
#define ENCODER_1_RESET             BIT(23)
#define ENCODER_2_ENABLE            BIT(24)
#define ENCODER_2_MD_ENABLE         BIT(25)
#define ENCODER_2_SF_ENABLE         BIT(26)
#define ENCODER_2_CFG_NUM           (BIT(28) + BIT(29() + BIT(30))
#define ENCODER_2_RESET             BIT(31)

#define SAREK_2022_6                BIT(0)
#define SAREK_2022_2                BIT(1)
#define SAREK_MB_PRESENT            BIT(2)
#define SAREK_IP_LIVE               BIT(3)
#define SAREK_2022_7                BIT(4)
#define SAREK_PTP_PLL				BIT(5)
#define SAREK_TX_TOP34              BIT(6)
#define SAREK_2110                  BIT(7)

#define SAREK_TX0_MASK              (BIT(31) + BIT(30) + BIT(29) + BIT(28))
#define SAREK_RX0_MASK              (BIT(27) + BIT(26) + BIT(25) + BIT(24))
#define SAREK_TX1_MASK              (BIT(23) + BIT(22) + BIT(21) + BIT(20))
#define SAREK_RX1_MASK              (BIT(19) + BIT(18) + BIT(17) + BIT(16))


#define SAREK_ReferenceFrameRate	(BIT(3) + BIT(2) + BIT(1) + BIT(0))
#define SAREK_ReferenceFrameLines	(BIT(6) + BIT(5) + BIT(4))
#define SAREK_ReferenceProgessive 	BIT(7)
#define SAREK_ReferenceSelected		(BIT(11) + BIT(10) + BIT(9) + BIT(8))
#define SAREK_GenlockLocked			BIT(12)
#define SAREK_Fractional_1			BIT(16)
#define SAREK_Fractional_2			BIT(17)
#define SAREK_Fractional_3			BIT(18)
#define SAREK_Fractional_4			BIT(19)

#define SAREK_LICENSE_PRESENT       BIT(31)
#define SAREK_LICENSE_VALID         BIT(30)
#define SAREK_LICENSE_J2K           BIT(1)

/////////////////////////////////////////////////////////////////////
//
// 10G Regsiters
//
/////////////////////////////////////////////////////////////////////

#define kReg10gemac_rx_bytes_lo                 128                     // 0x0200
#define kReg10gemac_rx_bytes_hi                 129                     // 0x0204
#define kReg10gemac_tx_bytes_lo                 130                     // 0x0208
#define kReg10gemac_tx_bytes_hi                 131                     // 0x020C

#define kReg10gemac_rx_frames_under_lo          132                     // 0x0210
#define kReg10gemac_rx_frames_under_hi          133                     // 0x0214
#define kReg10gemac_rx_frames_frag_lo           134                     // 0x0218
#define kReg10gemac_rx_frames_frag_hi           135                     // 0x021C

#define kReg10gemac_rx_frames_64_lo             136                     // 0x0220
#define kReg10gemac_rx_frames_64_hi             137                     // 0x0224
#define kReg10gemac_rx_frames_65_127_lo         138                     // 0x0228
#define kReg10gemac_rx_frames_65_127_hi         139                     // 0x022C
#define kReg10gemac_rx_frames_128_255_lo        140                     // 0x0230
#define kReg10gemac_rx_frames_128_255_hi        141                     // 0x0234
#define kReg10gemac_rx_frames_256_511_l0        142                     // 0x0238
#define kReg10gemac_rx_frames_256_511_hi        143                     // 0x023c
#define kReg10gemac_rx_frames_512_1023_lo       144                     // 0x0240
#define kReg10gemac_rx_frames_512_1023_hi       145                     // 0x0244
#define kReg10gemac_rx_frames_1024_max_lo       146                     // 0x0248
#define kReg10gemac_rx_frames_1024_max_hi       147                     // 0x024c
#define kReg10gemac_rx_frames_oversize_lo       148                     // 0x0250
#define kReg10gemac_rx_frames_oversize_hi       149                     // 0x0254

#define kReg10gemac_tx_frames_64_lo             150                     // 0x0258
#define kReg10gemac_tx_frames_64_hi             151                     // 0x025c
#define kReg10gemac_tx_frames_65_127_lo         152                     // 0x0260
#define kReg10gemac_tx_frames_65_127_hi         153                     // 0x0264
#define kReg10gemac_tx_frames_128_255_lo        154                     // 0x0268
#define kReg10gemac_tx_frames_128_255_hi        155                     // 0x026c
#define kReg10gemac_tx_frames_256_511_l0        156                     // 0x0270
#define kReg10gemac_tx_frames_256_511_hi        157                     // 0x0274
#define kReg10gemac_tx_frames_512_1023_lo       158                     // 0x0278
#define kReg10gemac_tx_frames_512_1023_hi       159                     // 0x027c
#define kReg10gemac_tx_frames_1024_max_lo       160                     // 0x0280
#define kReg10gemac_tx_frames_1024_max_hi       161                     // 0x0284
#define kReg10gemac_tx_frames_oversize_lo       162                     // 0x0288
#define kReg10gemac_tx_frames_oversize_hi       163                     // 0x028c

#define kReg10gemac_rx_frames_ok_lo             164                     // 0x0290
#define kReg10gemac_rx_frames_ok_hi             165                     // 0x0294

#define kReg10gemac_rx_crc_err_lo               166                     // 0x0298
#define kReg10gemac_rx_crc_err_hi               167                     // 0x029c
#define kReg10gemac_rx_frames_bdcast_lo         168                     // 0x02a0
#define kReg10gemac_rx_frames_bdcast_hi         169                     // 0x02a4

#define kReg10gemac_rx_frames_mcast_lo          170                     // 0x02a8
#define kReg10gemac_rx_frames_macst_hi          171                     // 0x02ac
#define kReg10gemac_rx_frames_ctrl_lo           172                     // 0x02b0
#define kReg10gemac_rx_frames_ctrt_hi           173                     // 0x02b4
#define kReg10gemac_rx_frames_len_err_lo        174                     // 0x02b8
#define kReg10gemac_rx_frames_len_err_hi        175                     // 0x02bc
#define kReg10gemac_rx_frames_vlan_lo           176                     // 0x02c0
#define kReg10gemac_rx_frames_vlan_hi           177                     // 0x02c4
#define kReg10gemac_rx_frames_pause_lo          178                     // 0x02c8
#define kReg10gemac_rx_frames_pause_hi          179                     // 0x02cc
#define kReg10gemac_rx_frames_op_lo             180                     // 0x02d0
#define kReg10gemac_rx_frames_op_hi             181                     // 0x02d4

#define kReg10gemac_tx_frames_ok_lo             182                     // 0x02d8
#define kReg10gemac_tx_frames_ok_hi             183                     // 0x02dc
#define kReg10gemac_tx_frames_bdcast_lo         184                     // 0x02e0
#define kReg10gemac_tx_frames_bdcast_hi         185                     // 0x02e4
#define kReg10gemac_tx_frames_mcast_lo          186                     // 0x02e8
#define kReg10gemac_tx_frames_mcast_hi          187                     // 0x02ec
#define kReg10gemac_tx_frames_underrun_lo       188                     // 0x02f0
#define kReg10gemac_tx_frames_underrun_hi       189                     // 0x02f4
#define kReg10gemac_tx_frames_ctrl_lo           190                     // 0x02f8
#define kReg10gemac_tx_frames_ctrt_hi           191                     // 0x02fc
#define kReg10gemac_tx_frames_vlan_lo           192                     // 0x0300
#define kReg10gemac_tx_frames_vlan_hi           193                     // 0x0304
#define kReg10gemac_tx_frames_pause_lo          194                     // 0x0308
#define kReg10gemac_tx_frames_pause_hi          195                     // 0x030c

#define kReg10gemac_tx_frames_pri_lo            196                     // 0x0310
#define kReg10gemac_tx_frames_pri_hi            197                     // 0x0314
#define kReg10gemac_rx_frames_pri_lo            198                     // 0x0318
#define kReg10gemac_rx_frames_pri_hi            199                     // 0x031c

#define kReg10gemac_rx_cfg_0                    256                     // 0x0400
#define kReg10gemac_rx_cfg_1                    257                     // 0x0404
#define kReg10gemac_tx_cfg                      258                     // 0x0408
#define kReg10gemac_flow_cfg                    259                     // 0x040c
#define kReg10gemac_reconcil_cfg                260                     // 0x0410
#define kReg10gemac_rx_mtu_cfg                  261                     // 0x0414
#define kReg10gemac_tx_mtu_cfg                  262                     // 0x0418

#define kReg10gemac_pri_0_quanta                288                     // 0x0480
#define kReg10gemac_pri_1_quanta                289                     // 0x0484
#define kReg10gemac_pri_2_quanta                290                     // 0x0488
#define kReg10gemac_pri_3_quanta                291                     // 0x048c
#define kReg10gemac_pri_4_quanta                292                     // 0x0490
#define kReg10gemac_pri_5_quanta                293                     // 0x0494
#define kReg10gemac_pri_6_quanta                294                     // 0x0498
#define kReg10gemac_pri_7_quanta                295                     // 0x049c
#define kReg10gemac_legacy_pause                296                     // 0x04a0

#define kReg10gemac_version                     318                     // 0x04f8
#define kReg10gemac_capability                  319                     // 0x04fc

/////////////////////////////////////////////////////////////////////
//
// Genlock SPI Regsiters
//
/////////////////////////////////////////////////////////////////////

#define GENL_SPI_RESET                          16              // 0x40
#define GENL_SPI_CONTROL                        24              // 0x60
#define GENL_SPI_STATUS                         25              // 0x64
#define GENL_SPI_WRITE                          26              // 0x68
#define GENL_SPI_READ                           27              // 0x6C
#define GENL_SPI_SLAVE_SELECT                   28              // 0x70

#define GENL_SPI_SET_ADDR_CMD                   0               // 0x00
#define GENL_SPI_WRITE_CMD                      16              // 0x40
#define GENL_SPI_READ_CMD                       32              // 0x80


/////////////////////////////////////////////////////////////////////
//
// PLL Regsiters
//
/////////////////////////////////////////////////////////////////////

#define kRegPll_Config                          0               // 0x0000
#define kRegPll_PDF_Binthresh                   1               // 0x0004
#define kRegPll_PDF_Binaddr                     2               // 0x0008
#define kRegPll_PDF_Bindata                     3               // 0x000c
#define kRegPll_Gain                            4               // 0x0010
#define kRegPll_SrcIp                           5               // 0x0014
#define kRegPll_SrcPort                         6               // 0x0018
#define kRegPll_DstIp                           7               // 0x001c
#define kRegPll_DstPort                         8               // 0x0020
#define kRegPll_Es_Pid                          9               // 0x0024
#define kRegPll_Match                           10              // 0x0028
#define kRegPll_DecVidStd                       11              // 0x002c
#define kRegPll_Bias                            12              // 0x0030
#define kRegPll_Status                          13              // 0x0034
#define kRegPll_PhaseError                      14              // 0x0038
#define kRegPll_Correction                      15              // 0x003c

#define kRegPll_PTP_PDF_Binthresh               0x10
#define kRegPll_PTP_PDF_Binaddr                 0x11
#define kRegPll_PTP_PDF_Bindata                 0x12
#define kRegPll_PTP_Gain                        0x13
#define kRegPll_PTP_LclMacLo                    0x14
#define kRegPll_PTP_LclMacHi                    0x15
#define kRegPll_PTP_LclIP                       0x16
#define kRegPll_PTP_LclUdp                      0x17
#define kRegPll_PTP_MstrIP                      0x18
#define kRegPll_PTP_MstrMcast                   0x19
#define kRegPll_PTP_EventUdp                    0x1a
#define kRegPll_PTP_GenUdp                      0x1b
#define kRegPll_PTP_Match                       0x1c
#define kRegPll_PTP_LclClkIdLo                  0x1d
#define kRegPll_PTP_LclClkIdHi                  0x1e
#define kRegPll_PTP_PhaseError                  0x1f
#define kRegPll_PTP_Correction                  0x20
#define kRegPll_PTP_FPptpSecHi                  0x21
#define kRegPll_PTP_FPptpSecLo                  0x22
#define kRegPll_PTP_FPptpNSec                   0x23
#define kRegPll_PTP_CurPtpSecHi                 0x24
#define kRegPll_PTP_CurPtpSecLo                 0x25
#define kRegPll_PTP_CurPtpNSec                  0x26
#define kRegPll_NTP_Timestamp                   0x27

/////////////////////////////////////////////////////////////////////
//
// AXI Lite Control Regsiters
//
/////////////////////////////////////////////////////////////////////

#define kRegAXI_Lite_Cntrl_Es_Pid               0
#define kRegAXI_Lite_Cntrl_Gain                 1
#define kRegAXI_Lite_Cntrl_Bias                 2
#define kRegAXI_Lite_Cntrl_BinThresh            3
#define kRegAXI_Lite_Cntrl_SrcIP                4
#define kRegAXI_Lite_Cntrl_DstIP                5
#define kRegAXI_Lite_Cntrl_Port                 6
#define kRegAXI_Lite_Cntrl_Match                7
#define kRegAXI_Lite_Cntrl_Pll_Reset            8
#define kRegAXI_Lite_Cntrl_Pll_Status           32

#endif // REGISTERS_MB_H