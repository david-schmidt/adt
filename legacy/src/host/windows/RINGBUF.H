class CRingBuffer
{
protected:
	LPSTR m_buf;
	int m_in;
	int m_out;
	int m_z;
public:
    CRingBuffer();
   ~CRingBuffer();

	BOOL Init (DWORD size);
	void Empty ();
	LPSTR PutBegin (DWORD * nBytes);
	void  PutEnd   (DWORD nBytes);
	LPSTR GetBegin (DWORD * nBytes);
	void  GetEnd   (DWORD nBytes);
};

inline void CRingBuffer::Empty ()
{ m_in = m_out = 0; }
