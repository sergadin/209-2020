#pragma once

namespace R2
{
	class Message
	{
		typedef unsigned char byte;

		size_t n;
		byte* data;
		const byte* readonly_data;

		// reading message

		size_t j0; // already bytes sent
		bool _transferCompleted;
		size_t z0;

		size_t encodeLength(byte* buf)
		{
			if (n <= 127)
			{
				*buf = (byte)n;
				return 1;
			}
			else
			{
				size_t k = 0, m = n;
				byte a[64];
				while (m > 0)
				{
					a[k++] = (byte)(m & 0xff);
					m = m >> 8;
				}
				auto r = k;
				*buf = 0x80 | (byte)r;
				for (k = 0; k < r; k++)
				{
					buf[r-k] = a[k];
				}
				return 1 + r;
			}
		}

	public:
		Message() : n(0), data(nullptr), j0(0), z0(0), _transferCompleted(false)
		{
		}

		void attachBlob(const byte* data_, size_t n_)
		{
			// TODO: check state!
			readonly_data = data_, n = n_;
			j0 = 0;
			_transferCompleted = false;
		}

		inline size_t send(size_t chunkSize, byte* buf) // chunkSize >= 64,   ~ 1024 .. 4096
		{
			if (_transferCompleted)
				return 0;
			//
			size_t r = 0;
			//
			if (j0 == 0)
			{
				r = encodeLength(buf); // 64: 3 + 61
				buf += r;
			}
			//
			size_t u = chunkSize - r;
			if (u >= n - j0)
			{
				_transferCompleted = true;
				u = n - j0;
			}
			// memcpy?
			for (size_t i = 0; i < u; i++, buf++)
			{
				*buf = readonly_data[j0 + i];
			}
			j0 += u;
			// cout << "r = " << r << "   u = " << u << endl;
			return r + u;
		}

		inline bool transferCompleted()
		{
			return _transferCompleted;
		}

		// ----
		// Receive

		size_t startReceiving(byte* buf)
		{
			// TODO: check state

			size_t r = 0;
			if ((*buf) & 0x80)
			{
				r = (*buf) & 0x7f;
				n = 0;
				for (size_t k = 0; k < r; k++)
				{
					n = (n << 8) | buf[k];
				}
			}
			else
			{
				n = (*buf) & 0x7f;
			}
			//
			j0 = 0;
			z0 = r + 1;
			data = new byte[n];

			_transferCompleted = false;
		}

		inline void receive(size_t len, byte* buf)
		{
			if (j0 == 0)
			{
				buf += z0;
				auto chunkSize = len - z0;
				if (chunkSize > n)
					chunkSize = n;
				for (size_t i = 0; i < chunkSize; i++, buf++)
					data[j0+i] = *buf;
				//
				j0 += chunkSize;
				// cout << "j0 = " << j0 << "   len = " << len << "   z0 = " << z0 << endl;
			}
			else
			{
				auto chunkSize = len;
				if (chunkSize > n-j0)
					chunkSize = n-j0;
				for (size_t i = 0; i < chunkSize; i++, buf++)
					data[j0+i] = *buf;
				//
				j0 += len;
			}
			if (j0 == n)
				_transferCompleted = true;
		}

		inline size_t length() const
		{
			return n;
		}

		inline const byte* binary() const
		{
			return data;
		}
	};
}