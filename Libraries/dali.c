void TI_DALI_Tx(unsigned char response)
{
  unsigned char bit_mask=0x80;
  /* Start Bit  */
  DALI_TX_PxOUT &= ~DALI_TX_BIT;
  __delay_cycles(416*MCLK_uSEC_COUNT);
  DALI_TX_PxOUT |= DALI_TX_BIT;
  __delay_cycles(416*MCLK_uSEC_COUNT);
  while(bit_mask)
  {
    if(bit_mask & response)
    { /* transmit a '1' */
        DALI_TX_PxOUT &= ~DALI_TX_BIT;
      __delay_cycles(416*MCLK_uSEC_COUNT);
      DALI_TX_PxOUT |= DALI_TX_BIT;
      __delay_cycles(416*MCLK_uSEC_COUNT);
    }
    else
    { /* transmit a '0' */
      DALI_TX_PxOUT |= DALI_TX_BIT;
      __delay_cycles(416*MCLK_uSEC_COUNT);
      DALI_TX_PxOUT &= ~DALI_TX_BIT;
      __delay_cycles(416*MCLK_uSEC_COUNT);
    }
    bit_mask = bit_mask >> 1;
  }
  DALI_TX_PxOUT |= DALI_TX_BIT; /* IDLE condition */
}