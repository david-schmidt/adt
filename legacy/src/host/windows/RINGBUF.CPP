#include "stdafx.h"
#include "ringbuf.h"

CRingBuffer::CRingBuffer ()
{
	m_buf = NULL;
}

CRingBuffer::~CRingBuffer ()
{
	if (m_buf) LocalFree (m_buf);
}

BOOL CRingBuffer::Init (DWORD size)
{
	BOOL ret = TRUE;
	m_in = 0;
	m_out = 0;
	if (m_buf) LocalFree (m_buf);
	m_buf = (LPSTR) LocalAlloc (LMEM_FIXED, size);
	if (!m_buf) ret = FALSE;
	else m_z = size;
	return ret;
}

LPSTR CRingBuffer::PutBegin (DWORD * nBytes)
{
	LPSTR ret = &m_buf[m_in];
	int max, n;

	max = m_out - 1; // single access to m_out keeps this class thread-safe
	if (m_in > max) max += m_z;
	if (max > m_z) max = m_z;
	n = max - m_in;

	if (*nBytes > (DWORD)n)
		*nBytes = (DWORD)n;

	return ret;
}

void CRingBuffer::PutEnd (DWORD n)
{
	m_in += n;
	ASSERT(m_in <= m_z);
	if (m_in >= m_z)
		m_in = 0;
}

LPSTR CRingBuffer::GetBegin (DWORD * nBytes)
{
	LPSTR ret = &m_buf[m_out];
	int max, n;

	max = m_in; // single access to m_in keeps this class thread-safe
	if (m_out > max) max += m_z;
	if (max > m_z) max = m_z;
	n = max - m_out;

	if (*nBytes > (DWORD)n)
		*nBytes = (DWORD)n;

	return ret;
}

void CRingBuffer::GetEnd (DWORD n)
{
	m_out += n;
	ASSERT(m_out <= m_z);
	if (m_out >= m_z)
		m_out = 0;
}
