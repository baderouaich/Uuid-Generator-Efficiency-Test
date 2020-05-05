# UUID Generator Efficiency Test
-------------------------------------------------
                    CPU Info
-------------------------------------------------
Vendor String       GenuineIntel
Vendor              Intel
Brand String        Intel(R) Core(TM)2 Duo CPU     E6550  @ 2.33GHz
Frequency           2327 MHz
Physical CPUs       1
CPU Cores           2
Logical CPUs        2
CPU Config          Single CPU, Dual Core, No Hyperthreading
--------------------------------------------------
                     RAM Info
--------------------------------------------------
|  Capacity  |  MemoryType |  Speed | TypeDetail |
--------------------------------------------------
| 2147483648 | 21          |   800  |  128       |
| 2147483648 | 21          |   800  |  128       |
| 2147483648 | 21          |   800  |  128       |
| 2147483648 | 21          |   800  |  128       |
| 4194304    | 11          |        |  4096      |

<br>
<pre>
Latest test on 05-05-2020 11:26pm
Build Type:			Release x64
Optimization:		ON
UUIDs Generated:	10,000,000 (100 Million)
UUIDs Repeated:		0
Test Duration:		14063 milliseconds
Memory Allocated due test: Each uuid has 37 bytes, so
37 * 10,000,000 uuids = 370,000,000‬ bytes
370,000,000‬ bytes / 1024 / 1024 = 352 MB

~352MB for vecUUDs which holds generated uuids
~352MB for m_RepeatedUUIDs which holds generated uuids minus repeated ones
~300MB for uuid generator library and other stuff

Approximately the entire test takes ~1GO of memory
</pre> 