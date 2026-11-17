
## 汇编CALL相关空白地址规划
>空白地址范围要求，至少需要800字节大小

| 空白地址偏移 | 用途 | 偏移范围 | 内存大小 |
| --- | --- | --- | --- |
| C_EMPTY_ADDRESS+0		| HOOK代码		| 地址范围0~0x30			| 48字节
| C_EMPTY_ADDRESS+0x30	| 空白分割		| 地址范围0x30~0x70		| 64字节
| C_EMPTY_ADDRESS+0x70	| 数据区域		| 地址范围0x70~0xC0		| 80字节			
| C_EMPTY_ADDRESS+0x70	| 汇编HOOK判断	| 地址范围0x70~0x74		| 4字节
| C_EMPTY_ADDRESS+0x74	| 汇编结果		| 地址范围0x74~0x78		| 4字节
| C_EMPTY_ADDRESS+0x80	| 特效CALL空白  	| 地址范围0x80~0x88		| 8字节
| C_EMPTY_ADDRESS+0x88	| 武器冰冻		| 地址范围0x88~0xA8		| 32字节
| C_EMPTY_ADDRESS+0xA8	| 未使用			| 地址范围0xA8~0xB0		| 8字节
| C_EMPTY_ADDRESS+0xB0	| 未使用			| 地址范围0xB0~0xB8		| 8字节
| C_EMPTY_ADDRESS+0xB8	| 未使用			| 地址范围0xB8~0xC0		| 8字节
| C_EMPTY_ADDRESS+0xC0	| 汇编地址		| 地址范围0xC0~0x2C0    	| 512字节
| C_EMPTY_ADDRESS+0X2C0 | 空白分割 		| 地址范围0x2C0~0x2D0		| 16字节
| C_EMPTY_ADDRESS+0X2D0 | 天域装备数据	| 地址范围0x2D0~0x528		| 600字节(实际592)
| C_EMPTY_ADDRESS+0X528 | -				| -						| -
| C_EMPTY_ADDRESS+0X1000 | 空白地址结束(注意该地址不在申请内存中，只是作为结束的标识)   | -						| -
