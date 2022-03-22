#include "sm3.h"

int main(int argc, char *argv[])
{
	unsigned char Hash[32]={0};
	//unsigned char Hash2[32] = "DEBE9FF92275B8A138604889C18E5A4D6FDB70E5387E5765293DCBA39CC5732";

//	unsigned char Hash[33]={0};
//	printf("%s\n",Hash);

	char* str="abc";
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	int len;
	len=strlen(str);
	if(!SM3(str, len, Hash))
		printf("1 false\n");
	char hcj_str[64];
	StringToChar(Hash, hcj_str);
	for(int i = 0; i < 64; ++i){
		printf("%c", hcj_str[i]);
	}
	printf("\n");
	
	


	char *str2 = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
	//debe9ff9 2275b8a1 38604889 c18e5a4d 6fdb70e5 387e5765 293dcba3 9c0c5732
	len=strlen(str2);
	if(!SM3(str2, len, Hash))
		printf("2 false\n");
	

	

	
	return 0;
}
