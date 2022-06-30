#include "RingBuffer.h"



Ring_ErrorStatus_t Ring_Init(Ring_Buffer_t *ring, uint8_t *buf, uint16_t size) 
{
		ring->size = size;
    ring->buffer = buf;
    Ring_Clear(ring);

    return ( ring->buffer ? RING_SUCCESS : RING_ERROR ); 
}


void Ring_Clear(Ring_Buffer_t* buf)
{
	buf->idxIn = 0; 
	buf->idxOut = 0; 
}	

uint16_t Ring_GetCount(const Ring_Buffer_t *buf)
{
	uint16_t result = 0; 
	if(buf->idxIn < buf->idxOut) { result = buf->size + buf->idxIn - buf->idxOut; }
	else { result = buf->idxIn - buf->idxOut; }
	return result; 
}


uint16_t Ring_CRC16ccitt(const Ring_Buffer_t *buf, uint16_t lenght, uint16_t position)
{
	return Ring_CRC16ccitt_Intermediate( buf, lenght, 0xFFFF, position);
}

uint16_t Ring_CRC16ccitt_Intermediate(const Ring_Buffer_t *buf, uint16_t lenght, uint16_t tmpCrc, uint16_t position)
{
	uint16_t crc = tmpCrc;
	uint16_t crctab;
	uint8_t byte;
	
    while (lenght--)
    {
        crctab = 0x0000;
        byte = (Ring_ShowSymbol( buf, lenght + position))^( crc >> 8 );
        if( byte & 0x01 ) crctab ^= 0x1021;
        if( byte & 0x02 ) crctab ^= 0x2042;
        if( byte & 0x04 ) crctab ^= 0x4084;
        if( byte & 0x08 ) crctab ^= 0x8108;
        if( byte & 0x10 ) crctab ^= 0x1231;
        if( byte & 0x20 ) crctab ^= 0x2462;
        if( byte & 0x40 ) crctab ^= 0x48C4;
        if( byte & 0x80 ) crctab ^= 0x9188;
        
        crc = ( ( (crc & 0xFF)^(crctab >> 8) ) << 8 ) | ( crctab & 0xFF );
    }
    return crc;
}



// view data bez udalenia
// symbol number - smeshenie otnositelno tochki chteniya
int32_t Ring_ShowSymbol(const Ring_Buffer_t *buf, uint16_t symbolNumber)
{
		uint32_t pointer = buf->idxOut + symbolNumber;
		int32_t retval = -1; 
		if(symbolNumber < Ring_GetCount(buf))
		{
				if(pointer > buf->size) pointer -=buf->size; 
			  retval = buf->buffer[pointer]; 
		}
		return retval; 
}

void Ring_Put(Ring_Buffer_t* buf, uint8_t symvol)
{
	buf->buffer[buf->idxIn++] = symvol;
	if(buf->idxIn >= buf->size) buf->idxIn = 0; 
}


void Ring_PutBuffr(Ring_Buffer_t *ringbuf, uint8_t *src, uint16_t len)
{
	while(len--) Ring_Put(ringbuf, *(src++));
}

uint8_t Ring_Pop(Ring_Buffer_t *buf)
{
	uint8_t result = buf->buffer[buf->idxOut++]; 
	if(buf->idxOut >= buf->size) buf->idxOut = 0; 
	return result;  
}


uint16_t Ring_Pop16(Ring_Buffer_t *buf)
{
		uint16_t retval = Ring_Pop(buf) << 8;
    retval += Ring_Pop(buf);
    return retval;

}

uint32_t Ring_Pop32(Ring_Buffer_t *buf)
{
		uint32_t retval  = Ring_Pop(buf) << 8;
    retval += Ring_Pop(buf) << 8;
    retval += Ring_Pop(buf) << 8;
    retval += Ring_Pop(buf);
    return retval;

}
void Ring_PopBuffr(Ring_Buffer_t *ringbuf, uint8_t *destination, uint16_t len)
{
		while(len--) *(destination++) = Ring_Pop(ringbuf);
}

void Ring_PopString(Ring_Buffer_t *ringbuf, char *string)
{
	while(Ring_ShowSymbol(ringbuf,0) > 0) *(string++) = Ring_Pop(ringbuf);
}
//*****************************************************************************************
