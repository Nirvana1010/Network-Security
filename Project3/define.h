
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))   //F function
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))	  //G function 
#define H(x, y, z) ((x) ^ (y) ^ (z))			  //H function 
#define I(x, y, z) ((y) ^ ((x) | (~z)))			  //I function 
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n)))) 

#define FF(a, b, c, d, x, s, ac) {   \
	(a) += F ((b), (c), (d)) + (x) + ac;   \
	(a) = ROTATE_LEFT ((a), (s));   \
	(a) += (b);  \
} 

#define GG(a, b, c, d, x, s, ac) {   \
	(a) += G ((b), (c), (d)) + (x) + ac;   \
	(a) = ROTATE_LEFT ((a), (s));   \
	(a) += (b);  \
} 

#define HH(a, b, c, d, x, s, ac) {   \
	(a) += H ((b), (c), (d)) + (x) + ac;   \
	(a) = ROTATE_LEFT ((a), (s));   \
	(a) += (b);  \
} 

#define II(a, b, c, d, x, s, ac) {   \
	(a) += I ((b), (c), (d)) + (x) + ac;   \
	(a) = ROTATE_LEFT ((a), (s));   \
	(a) += (b);  \
} 

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
