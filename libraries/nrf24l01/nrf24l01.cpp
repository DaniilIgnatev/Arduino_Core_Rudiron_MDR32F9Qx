// Modified 2022 by Daniil Ignatev for Arduino_Core_Rudiron_MDR32F9Qx

// Functions to manage the nRF24L01+ transceiver

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include "nrf24l01.h"

// The address used to test presence of the transceiver,
// note: should not exceed 5 bytes
#define nRF24_TEST_ADDR "nRF24"

// Addresses of the address registers
static const uint8_t nRF24_ADDR_REGS[7] = {
	nRF24_REG_RX_ADDR_P0,
	nRF24_REG_RX_ADDR_P1,
	nRF24_REG_RX_ADDR_P2,
	nRF24_REG_RX_ADDR_P3,
	nRF24_REG_RX_ADDR_P4,
	nRF24_REG_RX_ADDR_P5,
	nRF24_REG_TX_ADDR};

// Reads a value of register
// input:
//   reg - number of register to read
// return: value of register
static uint8_t nRF24_ReadReg(uint8_t reg)
{
	uint8_t value;

	nRF24_CSN_L;
	nRF24_LL_RW(reg & nRF24_MASK_REG_MAP);
	value = nRF24_LL_RW(nRF24_CMD_NOP);
	nRF24_CSN_H;

	return value;
}

// Writes a new value to the register
// input:
//   reg - number of register to write
//   value - value to write
static void nRF24_WriteReg(uint8_t reg, uint8_t value)
{
	nRF24_CSN_L;
	if (reg < nRF24_CMD_W_REGISTER)
	{
		// This is a register access
		nRF24_LL_RW((uint8_t)(nRF24_CMD_W_REGISTER | (reg & nRF24_MASK_REG_MAP)));
		nRF24_LL_RW(value);
	}
	else
	{
		// This is a single byte command or future command/register
		nRF24_LL_RW(reg);
		if ((reg != nRF24_CMD_FLUSH_TX) && (reg != nRF24_CMD_FLUSH_RX) &&
			(reg != nRF24_CMD_REUSE_TX_PL) && (reg != nRF24_CMD_NOP))
		{
			// Send register value
			nRF24_LL_RW(value);
		}
	}
	nRF24_CSN_H;
}

// Reads to the buffer from a multi-byte register
// input:
//   reg - number of register to read
//   pBuf - pointer to the buffer for register data
//   count - number of bytes to read
static void nRF24_ReadMBReg(uint8_t reg, uint8_t *pBuf, uint8_t count)
{
	nRF24_CSN_L;
	nRF24_LL_RW(reg);

	for (uint8_t i = 0; i < count; i++)
	{
		uint8_t newValue = nRF24_LL_RW(nRF24_CMD_NOP);
		*(pBuf + i) = newValue;
	}

	nRF24_CSN_H;
}

// Writes a buffer to a multi-byte register
// input:
//   reg - number of register to write
//   pBuf - pointer to the buffer with data to write
//   count - number of bytes to write
static void nRF24_WriteMBReg(uint8_t reg, uint8_t *pBuf, uint8_t count)
{
	nRF24_CSN_L;
	nRF24_LL_RW(reg);
	while (count--)
	{
		nRF24_LL_RW(*pBuf++);
	}
	nRF24_CSN_H;
}

// Configures the transceiver to its initial state
void nRF24_Init(void)
{
	// Write to the registers their initial values
	nRF24_WriteReg(nRF24_REG_CONFIG, 0x08);
	nRF24_WriteReg(nRF24_REG_EN_AA, 0x3F);
	nRF24_WriteReg(nRF24_REG_EN_RXADDR, 0x03);
	nRF24_WriteReg(nRF24_REG_SETUP_AW, 0x03);
	nRF24_WriteReg(nRF24_REG_SETUP_RETR, 0x03);
	nRF24_WriteReg(nRF24_REG_RF_CH, 0x02);
	nRF24_WriteReg(nRF24_REG_RF_SETUP, 0x0E);
	nRF24_WriteReg(nRF24_REG_STATUS, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P0, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P1, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P2, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P3, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P4, 0x00);
	nRF24_WriteReg(nRF24_REG_RX_PW_P5, 0x00);
	nRF24_WriteReg(nRF24_REG_DYNPD, 0x00);
	nRF24_WriteReg(nRF24_REG_FEATURE, 0x00);

	// Configure the default RX/TX address values
	uint8_t addr[5];
	uint8_t idx;
	for (idx = 0U; idx < sizeof(addr); idx++)
	{
		addr[idx] = 0xE7;
	}
	nRF24_SetAddr(nRF24_PIPETX, addr);
	nRF24_SetAddr(nRF24_PIPE0, addr);
	for (idx = 0U; idx < sizeof(addr); idx++)
	{
		addr[idx] = 0xC2;
	}
	nRF24_SetAddr(nRF24_PIPE1, addr);
	for (idx = 2U; idx < 6U; idx++)
	{
		addr[0] = idx + 0xC1;
		nRF24_SetAddr(idx, addr);
	}

	// Clear the FIFO's
	nRF24_FlushRX();
	nRF24_FlushTX();

	// Clear any pending interrupt flags
	nRF24_ClearIRQFlags();

	// Deassert CSN pin (chip release)
	nRF24_CSN_H;
}

// Checks the presence of the nRF24L01
// return:
//   zero - transceiver is absent
//   non-zero: transceiver is present
uint8_t nRF24_Check(void)
{
	uint8_t rxbuf[sizeof(nRF24_TEST_ADDR) - 1U];
	uint8_t *ptr = (uint8_t *)nRF24_TEST_ADDR;
	uint8_t idx;

	// Write the test address to the TX_ADDR register
	nRF24_WriteMBReg(
		nRF24_CMD_W_REGISTER | nRF24_REG_TX_ADDR,
		ptr,
		sizeof(nRF24_TEST_ADDR) - 1U);

	// Read it back to the buffer
	nRF24_ReadMBReg(
		nRF24_CMD_R_REGISTER | nRF24_REG_TX_ADDR,
		rxbuf,
		sizeof(nRF24_TEST_ADDR) - 1U);

	// Compare transmitted and received data...
	for (idx = 0U; idx < sizeof(nRF24_TEST_ADDR) - 1U; idx++)
	{
		if (rxbuf[idx] != *ptr++)
		{
			// The transceiver is absent
			return 0U;
		}
	}

	// The transceiver is present
	return !0U;
}

// Set transceiver power mode
// input:
//   mode - new state of power mode, one of nRF24_PWR_xx values
void nRF24_SetPowerMode(uint8_t mode)
{
	uint8_t reg;

	reg = nRF24_ReadReg(nRF24_REG_CONFIG);
	if (mode == nRF24_PWR_UP)
	{
		// Set the PWR_UP bit of CONFIG register to wake the transceiver
		// It goes into Standby-I mode with consumption about 26uA
		reg |= nRF24_CONFIG_PWR_UP;
	}
	else
	{
		// Clear the PWR_UP bit of CONFIG register to put the transceiver
		// It goes into Power Down mode with consumption about 900nA
		reg &= (uint8_t)(~nRF24_CONFIG_PWR_UP);
	}
	nRF24_WriteReg(nRF24_REG_CONFIG, reg);
}

// Set transceiver operational mode
// input:
//   mode - operational mode, one of nRF24_MODE_xx values
void nRF24_SetOperationalMode(uint8_t mode)
{
	uint8_t reg;

	// Configure PRIM_RX bit of the CONFIG register
	reg = nRF24_ReadReg(nRF24_REG_CONFIG);
	reg &= (uint8_t)(~nRF24_CONFIG_PRIM_RX);
	reg = (uint8_t)(reg | (mode & nRF24_CONFIG_PRIM_RX));
	nRF24_WriteReg(nRF24_REG_CONFIG, reg);
}

// Configure transceiver CRC scheme
// input:
//   scheme - CRC scheme, one of nRF24_CRC_xx values
// note: transceiver will forcibly turn on the CRC in case if auto acknowledgment
//       enabled for at least one RX pipe
void nRF24_SetCRCScheme(uint8_t scheme)
{
	uint8_t reg;

	// Configure EN_CRC[3] and CRCO[2] bits of the CONFIG register
	reg = nRF24_ReadReg(nRF24_REG_CONFIG);
	reg &= (uint8_t)(~nRF24_MASK_CRC);
	reg = (uint8_t)(reg | (scheme & nRF24_MASK_CRC));
	nRF24_WriteReg(nRF24_REG_CONFIG, reg);
}

// Set RF frequency channel
// input:
//   channel - radio frequency channel, can be a value from 0 to 127
// note: frequency will be (2400 + channel)MHz
// note: PLOS_CNT[7:4] bits of the OBSERVER_TX register will be reset
void nRF24_SetRFChannel(uint8_t channel)
{
	nRF24_WriteReg(nRF24_REG_RF_CH, channel);
}

// Set automatic retransmission parameters
// input:
//   ard - auto retransmit delay, one of nRF24_ARD_xx values
//   arc - count of auto retransmits, value form 0 to 15
// note: in order to disable automatic retransmission,
//       the value of the variable XXX should be zero
void nRF24_SetAutoRetr(uint8_t ard, uint8_t arc)
{
	nRF24_WriteReg(
		nRF24_REG_SETUP_RETR,
		(uint8_t)((ard << 4) | (arc & nRF24_MASK_RETR_ARC)));
}

// Set of address widths
// input:
//   addr_width - RX/TX address field width, value from 3 to 5
// note: this setting is common for all pipes
void nRF24_SetAddrWidth(uint8_t addr_width)
{
	nRF24_WriteReg(nRF24_REG_SETUP_AW, (uint8_t)(addr_width - 2U));
}

// Set static RX address for a specified pipe
// input:
//   pipe - pipe to configure address, one of nRF24_PIPEx values
//   addr - pointer to the buffer with address
// note: pipe can be a number from 0 to 5 (RX pipes) and 6 (TX pipe)
// note: the length of the buffer for pipes 0 and 1 must not be less
//       of the actual address width configured in the transceiver
// note: for the pipes[2..5] only the first byte from the addr buffer
//       will be written since the pipes[1..5] share the four
//       most significant address bytes
// note: always ensure that none of the data pipes have the same address
void nRF24_SetAddr(uint8_t pipe, const uint8_t *addr)
{
	uint8_t addr_width;

	// RX_ADDR_Px register
	switch (pipe)
	{
	case nRF24_PIPETX:
	case nRF24_PIPE0:
	case nRF24_PIPE1:
		// Get address width
		addr_width = nRF24_GetAddrWidth();
#if (!nRF24_ADDR_REVERSE)
		nRF24_WriteMBReg(
			nRF24_CMD_W_REGISTER | nRF24_ADDR_REGS[pipe],
			(uint8_t *)addr,
			addr_width);
#else
		// Write address in reverse order
		nRF24_CSN_L;
		nRF24_LL_RW(nRF24_CMD_W_REGISTER | nRF24_ADDR_REGS[pipe]);
		while (addr_width--)
		{
			nRF24_LL_RW(*(addr + addr_width));
		}
		nRF24_CSN_H;
#endif // nRF24_ADDR_REVERSE
		break;
	case nRF24_PIPE2:
	case nRF24_PIPE3:
	case nRF24_PIPE4:
	case nRF24_PIPE5:
		// Write first byte from the addr buffer,
		// it will be the LSByte of the pipe address
		nRF24_WriteReg(nRF24_ADDR_REGS[pipe], *addr);
		break;
	default:
		// Incorrect pipe number -> do nothing
		break;
	}
}

// Configure RF output power in TX mode
// input:
//   tx_pwr - RF output power, one of nRF24_TXPWR_xx values
void nRF24_SetTXPower(uint8_t tx_pwr)
{
	uint8_t reg;

	// Configure RF_PWR[2:1] bits of the RF_SETUP register
	reg = nRF24_ReadReg(nRF24_REG_RF_SETUP);
	reg &= (uint8_t)(~nRF24_MASK_RF_PWR);
	reg |= tx_pwr;
	nRF24_WriteReg(nRF24_REG_RF_SETUP, reg);
}

// Configure transceiver data rate
// input:
//   data_rate - data rate, one of nRF24_DR_xx values
void nRF24_SetDataRate(uint8_t data_rate)
{
	uint8_t reg;

	// Configure RF_DR_LOW[5] and RF_DR_HIGH[3] bits of the RF_SETUP register
	reg = nRF24_ReadReg(nRF24_REG_RF_SETUP);
	reg &= (uint8_t)(~nRF24_MASK_DATARATE);
	reg |= data_rate;
	nRF24_WriteReg(nRF24_REG_RF_SETUP, reg);
}

// Configure a specified RX pipe
// input:
//   pipe - number of the RX pipe, value from 0 to 5
//   aa_state - state of auto acknowledgment, one of nRF24_AA_xx values
//   payload_len - payload length in bytes
void nRF24_SetRXPipe(uint8_t pipe, uint8_t aa_state, uint8_t payload_len)
{
	uint8_t reg;

	// Enable the specified pipe (EN_RXADDR register)
	reg = (nRF24_ReadReg(nRF24_REG_EN_RXADDR) | (1U << pipe)) & nRF24_MASK_EN_RX;
	nRF24_WriteReg(nRF24_REG_EN_RXADDR, reg);

	// Set RX payload length (RX_PW_Px register)
	nRF24_WriteReg(nRF24_REG_RX_PW_P0 + pipe, payload_len & nRF24_MASK_RX_PW);

	// Set auto acknowledgment for a specified pipe (EN_AA register)
	reg = nRF24_ReadReg(nRF24_REG_EN_AA);
	if (aa_state == nRF24_AA_ON)
	{
		reg |= (uint8_t)(1U << pipe);
	}
	else
	{
		reg &= (uint8_t) ~(1U << pipe);
	}
	nRF24_WriteReg(nRF24_REG_EN_AA, reg);
}

// Disable specified RX pipe
// input:
//   PIPE - number of RX pipe, value from 0 to 5
void nRF24_ClosePipe(uint8_t pipe)
{
	uint8_t reg;

	reg = nRF24_ReadReg(nRF24_REG_EN_RXADDR);
	reg &= (uint8_t) ~(1U << pipe);
	reg &= nRF24_MASK_EN_RX;
	nRF24_WriteReg(nRF24_REG_EN_RXADDR, reg);
}

// Enable the auto retransmit (a.k.a. enhanced ShockBurst) for the specified RX pipe
// input:
//   pipe - number of the RX pipe, value from 0 to 5
void nRF24_EnableAA(uint8_t pipe)
{
	uint8_t reg;

	// Set bit in EN_AA register
	reg = nRF24_ReadReg(nRF24_REG_EN_AA);
	reg = reg | (uint8_t)(1U << pipe);
	nRF24_WriteReg(nRF24_REG_EN_AA, reg);
}

// Disable the auto retransmit (a.k.a. enhanced ShockBurst) for one or all RX pipes
// input:
//   pipe - number of the RX pipe, value from 0 to 5, any other value will disable AA for all RX pipes
void nRF24_DisableAA(uint8_t pipe)
{
	if (pipe > 5U)
	{
		// Disable Auto-ACK for ALL pipes
		nRF24_WriteReg(nRF24_REG_EN_AA, 0x00);
	}
	else
	{
		// Clear bit in the EN_AA register
		uint8_t reg;
		reg = nRF24_ReadReg(nRF24_REG_EN_AA);
		reg &= (uint8_t) ~(1U << pipe);
		nRF24_WriteReg(nRF24_REG_EN_AA, reg);
	}
}

// Get address length
// return: the length of the address configured in the transceiver,
//         value from 3 to 5
uint8_t nRF24_GetAddrWidth(void)
{
	return nRF24_ReadReg(nRF24_REG_SETUP_AW) + 2U;
}

// Get value of the STATUS register
// return: value of STATUS register
uint8_t nRF24_GetStatus(void)
{
	return nRF24_ReadReg(nRF24_REG_STATUS);
}

// Get pending IRQ flags
// return: current status of RX_DR, TX_DS and MAX_RT bits of the STATUS register
uint8_t nRF24_GetIRQFlags(void)
{
	return (nRF24_ReadReg(nRF24_REG_STATUS) & nRF24_MASK_STATUS_IRQ);
}

// Get status of the RX FIFO
// return: one of the nRF24_STATUS_RXFIFO_xx values
uint8_t nRF24_GetStatus_RXFIFO(void)
{
	return (nRF24_ReadReg(nRF24_REG_FIFO_STATUS) & nRF24_MASK_RXFIFO);
}

// Get status of the TX FIFO
// return: one of the nRF24_STATUS_TXFIFO_xx values
// note: the TX_REUSE bit ignored
uint8_t nRF24_GetStatus_TXFIFO(void)
{
	return ((nRF24_ReadReg(nRF24_REG_FIFO_STATUS) & nRF24_MASK_TXFIFO) >> 4);
}

// Get pipe number for the payload available for reading from RX FIFO
// return: pipe number or 0x07 if the RX FIFO is empty
uint8_t nRF24_GetRXSource(void)
{
	return ((nRF24_ReadReg(nRF24_REG_STATUS) & nRF24_MASK_RX_P_NO) >> 1);
}

// Get auto retransmit statistic
// return: value of OBSERVE_TX register which contains two counters encoded in nibbles:
//   high - lost packets count (max value 15, can be reseted by write to RF_CH register)
//   low  - retransmitted packets count (max value 15, reseted when new transmission starts)
uint8_t nRF24_GetRetransmitCounters(void)
{
	return (nRF24_ReadReg(nRF24_REG_OBSERVE_TX));
}

// Reset packet lost counter (PLOS_CNT bits in OBSERVER_TX register)
void nRF24_ResetPLOS(void)
{
	uint8_t reg;

	// The PLOS counter is reset after write to RF_CH register
	reg = nRF24_ReadReg(nRF24_REG_RF_CH);
	nRF24_WriteReg(nRF24_REG_RF_CH, reg);
}

// Flush the TX FIFO
void nRF24_FlushTX(void)
{
	nRF24_WriteReg(nRF24_CMD_FLUSH_TX, nRF24_CMD_NOP);
}

// Flush the RX FIFO
void nRF24_FlushRX(void)
{
	nRF24_WriteReg(nRF24_CMD_FLUSH_RX, nRF24_CMD_NOP);
}

// Clear any pending IRQ flags
void nRF24_ClearIRQFlags(void)
{
	uint8_t reg;

	// Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register
	reg = nRF24_ReadReg(nRF24_REG_STATUS);
	reg |= nRF24_MASK_STATUS_IRQ;
	nRF24_WriteReg(nRF24_REG_STATUS, reg);
}

void nRF24_maskIRQ(uint8_t tx, uint8_t fail, uint8_t rx)
{
	uint8_t config_reg = nRF24_ReadReg(nRF24_REG_CONFIG);
	// clear the interrupt flags
	config_reg &= ~(1 << 4 | 1 << 5 | 1 << 6);
	// set the s[ecified interrupt flags
	config_reg |= fail << 4 | tx << 5 | rx << 6;
	nRF24_WriteReg(nRF24_REG_CONFIG, config_reg);
}

// Low level SPI transmit/receive function (hardware depended)
// input:
//   data - value to transmit via SPI
// return: value received from SPI
uint8_t nRF24_LL_RW(uint8_t data)
{
	return Rudiron::SSP::getSSP2().read_write(data);
}

// Write TX payload
// input:
//   pBuf - pointer to the buffer with payload data
//   length - payload length in bytes
void nRF24_WritePayload(uint8_t *pBuf, uint8_t length)
{
	nRF24_WriteMBReg(nRF24_CMD_W_TX_PAYLOAD, pBuf, length);
}

// Read top level payload available in the RX FIFO
// input:
//   pBuf - pointer to the buffer to store a payload data
//   length - pointer to variable to store a payload length
// return: one of nRF24_RX_xx values
//   nRF24_RX_PIPEX - packet has been received from the pipe number X
//   nRF24_RX_EMPTY - the RX FIFO is empty
nRF24_RXResult nRF24_ReadPayload(uint8_t *pBuf, uint8_t *length)
{
	uint8_t pipe;

	// Extract a payload pipe number from the STATUS register
	pipe = (nRF24_ReadReg(nRF24_REG_STATUS) & nRF24_MASK_RX_P_NO) >> 1;

	// RX FIFO empty?
	if (pipe < 6U)
	{
		// Get payload length
		*length = nRF24_ReadReg(nRF24_REG_RX_PW_P0 + pipe);

		// Read a payload from the RX FIFO
		if (*length)
		{
			nRF24_ReadMBReg(nRF24_CMD_R_RX_PAYLOAD, pBuf, *length);
		}

		return ((nRF24_RXResult)pipe);
	}

	// The RX FIFO is empty
	*length = 0U;

	return nRF24_RX_EMPTY;
}

#if 0

#define __printf(...) USART_printf(USART1, __VA_ARGS__)

// Print nRF24L01+ current configuration (for debug purposes)
void nRF24_DumpConfig(void) {
	uint8_t i, j;
	uint8_t aw;
	uint8_t buf[5];

	// Dump nRF24L01+ configuration

	// CONFIG
	i = nRF24_ReadReg(nRF24_REG_CONFIG);
	__printf("[0x%02X][0x%02X] MASK:%03b CRC:%02b PWR:%s MODE:P%s\r\n",
			nRF24_REG_CONFIG,
			i,
			i >> 4,
			(i & 0x0c) >> 2,
			(i & 0x02) ? "ON" : "OFF",
			(i & 0x01) ? "RX" : "TX"
		);

	// EN_AA
	i = nRF24_ReadReg(nRF24_REG_EN_AA);
	__printf("[0x%02X][0x%02X] ENAA: ", nRF24_REG_EN_AA, i);
	for (j = 0; j < 6; j++) {
		__printf("[P%1u%s]%s",
				j,
				(i & (1 << j)) ? "+" : "-",
				(j == 5) ? "\r\n" : " "
			);
	}

	// EN_RXADDR
	i = nRF24_ReadReg(nRF24_REG_EN_RXADDR);
	__printf("[0x%02X][0x%02X] EN_RXADDR: ", nRF24_REG_EN_RXADDR, i);
	for (j = 0; j < 6; j++) {
		__printf("[P%1u%s]%s", j,
				(i & (1 << j)) ? "+" : "-",
				(j == 5) ? "\r\n" : " "
			);
	}

	// SETUP_AW
	i = nRF24_ReadReg(nRF24_REG_SETUP_AW);
	aw = nRF24_GetAddrWidth();
	__printf("[0x%02X][0x%02X] Address: %u Bytes\r\n",
			nRF24_REG_SETUP_AW,
			i,
			aw
		);

	// SETUP_RETR
	i = nRF24_ReadReg(nRF24_REG_SETUP_RETR);
	__printf("[0x%02X][0x%02X] ARD=%04b ARC=%04b (retr.delay=%uus, count=%u)\r\n",
			nRF24_REG_SETUP_RETR,
			i,
			i >> 4,
			i & 0x0F,
			((i >> 4) * 250) + 250,
			i & 0x0F
		);

	// RF_CH
	i = nRF24_ReadReg(nRF24_REG_RF_CH);
	__printf("[0x%02X][0x%02X] RF channel:%u (%.3uGHz)\r\n",
			nRF24_REG_RF_CH,
			i,
			i,
			2400 + i
		);

	// RF_SETUP
	i = nRF24_ReadReg(nRF24_REG_RF_SETUP);
	__printf("[0x%02X][0x%02X] CONT_WAVE:%s PLL_LOCK:%s DataRate=",
			nRF24_REG_RF_SETUP,
			i,
			(i & 0x80) ? "ON" : "OFF",
			(i & 0x80) ? "ON" : "OFF"
		);
	switch ((i & 0x28) >> 3) {
		case 0x00:
			__printf("1M");
			break;
		case 0x01:
			__printf("2M");
			break;
		case 0x04:
			__printf("250k");
			break;
		default:
			__printf("???");
			break;
	}
	__printf("pbs RF_PWR=");
	switch ((i & 0x06) >> 1) {
		case 0x00:
			__printf("-18");
			break;
		case 0x01:
			__printf("-12");
			break;
		case 0x02:
			__printf("-6");
			break;
		case 0x03:
			__printf("0");
			break;
		default:
			__printf("???");
			break;
	}
	__printf("dBm\r\n");

	// STATUS
	i = nRF24_ReadReg(nRF24_REG_STATUS);
	__printf("[0x%02X][0x%02X] IRQ:%03b RX_PIPE:%u TX_FULL:%s\r\n",
			nRF24_REG_STATUS,
			i,
			(i & 0x70) >> 4,
			(i & 0x0E) >> 1,
			(i & 0x01) ? "YES" : "NO"
		);
	// OBSERVE_TX
	i = nRF24_ReadReg(nRF24_REG_OBSERVE_TX);
	__printf("[0x%02X][0x%02X] PLOS_CNT=%u ARC_CNT=%u\r\n",
			nRF24_REG_OBSERVE_TX,
			i,
			i >> 4,
			i & 0x0F
		);
	// RPD
	i = nRF24_ReadReg(nRF24_REG_RPD);
	__printf("[0x%02X][0x%02X] RPD=%s\r\n",
			nRF24_REG_RPD,
			i,
			(i & 0x01) ? "YES" : "NO"
		);

	// TX_ADDR
	nRF24_ReadMBReg(nRF24_REG_TX_ADDR, buf, aw);
	__printf("[0x%02X] TX_ADDR:    ", nRF24_REG_TX_ADDR);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P0
	nRF24_ReadMBReg(nRF24_REG_RX_ADDR_P0, buf, aw);
	__printf("[0x%02X] RX_ADDR_P0: ", nRF24_REG_RX_ADDR_P0);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P1
	nRF24_ReadMBReg(nRF24_REG_RX_ADDR_P1, buf, aw);
	__printf("[0x%02X] RX_ADDR_P1: ", nRF24_REG_RX_ADDR_P1);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P2
	buf[aw - 1] = nRF24_ReadReg(nRF24_REG_RX_ADDR_P2);
	__printf("[0x%02X] RX_ADDR_P2: ", nRF24_REG_RX_ADDR_P2);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P3
	buf[aw - 1] = nRF24_ReadReg(nRF24_REG_RX_ADDR_P3);
	__printf("[0x%02X] RX_ADDR_P3: ", nRF24_REG_RX_ADDR_P3);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P4
	buf[aw - 1] = nRF24_ReadReg(nRF24_REG_RX_ADDR_P4);
	__printf("[0x%02X] RX_ADDR_P4: ", nRF24_REG_RX_ADDR_P4);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_ADDR_P5
	buf[aw - 1] = nRF24_ReadReg(nRF24_REG_RX_ADDR_P5);
	__printf("[0x%02X] RX_ADDR_P5: ", nRF24_REG_RX_ADDR_P5);
	for (i = 0; i < aw; i++) { __printf("0x%02x ", buf[i]); }
	__printf("[");
	for (i = 0; i < aw; i++) { __printf("%c", buf[i]); }
	__printf("]\r\n");

	// RX_PW_P0
	__printf("[0x%02X] RX_PW_P0=%u\r\n", nRF24_REG_RX_PW_P0, nRF24_ReadReg(nRF24_REG_RX_PW_P0));
	// RX_PW_P1
	__printf("[0x%02X] RX_PW_P1=%u\r\n", nRF24_REG_RX_PW_P1, nRF24_ReadReg(nRF24_REG_RX_PW_P1));
	// RX_PW_P2
	__printf("[0x%02X] RX_PW_P2=%u\r\n", nRF24_REG_RX_PW_P2, nRF24_ReadReg(nRF24_REG_RX_PW_P2));
	// RX_PW_P3
	__printf("[0x%02X] RX_PW_P3=%u\r\n", nRF24_REG_RX_PW_P3, nRF24_ReadReg(nRF24_REG_RX_PW_P3));
	// RX_PW_P4
	__printf("[0x%02X] RX_PW_P4=%u\r\n", nRF24_REG_RX_PW_P4, nRF24_ReadReg(nRF24_REG_RX_PW_P4));
	// RX_PW_P5
	__printf("[0x%02X] RX_PW_P5=%u\r\n", nRF24_REG_RX_PW_P5, nRF24_ReadReg(nRF24_REG_RX_PW_P5));
}

#endif // nRF24_DumpConfig()

// Configure the GPIO lines of the nRF24L01 transceiver
// note: IRQ pin must be configured separately
void nRF24_GPIO_Init(void)
{
	PORT_InitTypeDef PORT_InitStructure;

	// Enable the nRF24L01 GPIO peripherals
	RST_CLK_PCLKcmd(nRF24_GPIO_PERIPHERALS, ENABLE);

	// Configure CSN pin
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_Pin = nRF24_CSN_PIN;

	PORT_Init(nRF24_CSN_PORT, &PORT_InitStructure);
	nRF24_CSN_H;

	// Configure CE pin
	PORT_InitStructure.PORT_Pin = nRF24_CE_PIN;
	PORT_Init(nRF24_CE_PORT, &PORT_InitStructure);
	nRF24_CE_L;
}

void nRF24_GPIO_DeInit(void)
{
	PORT_InitTypeDef PORT_InitStructure;

	// Configure CSN pin
	PORT_InitStructure.PORT_Pin = nRF24_CSN_PIN;
	PORT_Init(nRF24_CSN_PORT, &PORT_InitStructure);

	// Configure CE pin
	PORT_InitStructure.PORT_Pin = nRF24_CE_PIN;
	PORT_Init(nRF24_CE_PORT, &PORT_InitStructure);
}

// Used for not stuck waiting for IRQ
#define nRF24_WAIT_TIMEOUT 10000

// Function to transmit data packet
// input:
//   pBuf - pointer to the buffer with data to transmit
//   length - length of the data buffer in bytes
// return: one of nRF24_TX_xx values
nRF24_TXResult nRF24_TransmitPacket(uint8_t *pBuf, uint8_t length)
{
	volatile uint32_t wait = nRF24_WAIT_TIMEOUT;
	uint8_t status;

	// Deassert the CE pin (in case if it still high)
	nRF24_CE_L;

	uint8_t payload[32];
	for (uint8_t i = 0; i < length; i++)
	{
		payload[i] = pBuf[i];
	}
	
	// Transfer a data from the specified buffer to the TX FIFO
	nRF24_WritePayload(payload, length);

	// Start a transmission by asserting CE pin (must be held at least 10us)
	nRF24_CE_H;

	// Poll the transceiver status register until one of the following flags will be set:
	//   TX_DS  - means the packet has been transmitted
	//   MAX_RT - means the maximum number of TX retransmits happened
	// note: this solution is far from perfect, better to use IRQ instead of polling the status
	// do
	// {
	// 	status = nRF24_GetStatus();
	// 	if (status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT))
	// 	{
	// 		break;
	// 	}
	// } while (wait--);

	uint32_t startMicros = Rudiron::CLK::micros();
	status = nRF24_GetStatus();
    while (!(status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT)))
    {
		status = nRF24_GetStatus();
        if ((Rudiron::CLK::micros() - startMicros) > nRF24_WAIT_TIMEOUT)
        {
            break;
        }
    }

	// Deassert the CE pin (Standby-II --> Standby-I)
	nRF24_CE_L;

	if (!wait)
	{
		// Timeout
		return nRF24_TX_TIMEOUT;
	}

	// Clear pending IRQ flags
	nRF24_ClearIRQFlags();

	if (status & nRF24_FLAG_MAX_RT)
	{
		// Auto retransmit counter exceeds the programmed maximum limit (FIFO is not removed)
		return nRF24_TX_MAXRT;
	}

	if (status & nRF24_FLAG_TX_DS)
	{
		// Successful transmission
		return nRF24_TX_SUCCESS;
	}

	// Some banana happens, a payload remains in the TX FIFO, flush it
	nRF24_FlushTX();

	return nRF24_TX_ERROR;
}

void nRF24_bufferPayload()
{
	uint8_t nRF24_payload[NRF24_PAYLOAD_LENGTH];
	nRF24_RXResult pipe = nRF24_RX_EMPTY;

	// Length of received payload
	uint8_t payload_length;
	if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY)
	{
		// Get a payload from the transceiver
		pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);
	}

	if (payload_length != NRF24_PAYLOAD_LENGTH)
	{
		// ошибка, размер нагрузки не совпадает
		return;
	}

	uint8_t package_length = nRF24_payload[0];

	// Заполнение кольцевого буфера
	if (pipe != nRF24_RX_EMPTY)
	{
		NRF24_BUFFER_INDEX_T old_head = _nrf24_rx_buffer_head;

		for (uint8_t i = 1; i < package_length + 1; i++)
		{
			uint8_t data = nRF24_payload[i];

			NRF24_BUFFER_INDEX_T next_head = _nrf24_rx_buffer_head + 1;
			if (next_head == NRF24_RX_BUFFER_LENGTH)
			{
				next_head = 0;
			}

			if (next_head != _nrf24_rx_buffer_tail)
			{
				_nrf24_rx_buffer[_nrf24_rx_buffer_head] = data;
				_nrf24_rx_buffer_head = next_head;
			}
			else
			{
				_nrf24_rx_buffer_head = old_head;
				break;
			}
		}
	}
}
