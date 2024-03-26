DVGB02, vt23 Datakommunikation 1
Laboration 1
Oskar Backman Bengtén & Henrik Hultgren
----
2023-02-17

Laborationen består av tre program; webb_labb1.c, server.c och client.c
där webb_lab1.c innehåller programkod för webbservern medan server.c och client.c
innehåller programkod för tidsservern.

För att kompilera respektive program så används
[anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Webb$		gcc -o webbserver webb_labb1.c
[anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Tid$ 		gcc -o timeserver server.c
[anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Tid$ 		gcc -o timeclient client.c

För att exekvera webbservern så används
[anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Webb$		./webbserver

För att exekvera tidsserver (och client) så används
TERMINAL 1: [anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Tid$		./timeserver <PORT>
TERMINAL 2: [anvID@Exempelmaskin]../labb_1_labbgrupp_36/Lab1/Tid$		./timeclient <PORT>

*För att använda portar under 1024 så krävs extra behörighet*




