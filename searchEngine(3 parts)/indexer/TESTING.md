TESTING for indexer.c 
Alex Wolf, July 2016

### Test noticing when the wrong parameters were enetered:

//incorrect param numbers
1. wolfiesmac:indexer alexwolf$ ./indexer 1 1 1
ERROR: Please use 2 paramters to the indexer command.

2. wolfiesmac:indexer alexwolf$ ./indexer invalid 1 
ERROR: Directory invalid does not exist.

3. Correct params
wolfiesmac:indexer alexwolf$ ./indexer ../data 1
wolfiesmac:indexer alexwolf$ 

###getting proper data in hashtable countersset
-Doesn't include all data; too much for page

wolfiesmac:indexer alexwolf$ ./indexer ../data hi
   0: [key-directoryusedbyinboundsshconnections |[47=1, ], key-theshellallowsustodefine |[47=1, ], key-suspended |[46=1, ], key-aprogramreadinganemptypipewillbe |[46=1, ], key-pipeswereinventedbydougmcilroywhilehewasworkingwithkenthompsonanddennisritchieatat |[46=1, ], key-thenthecommandexpectsinputfromthestandardinput |[46=1, ], key-justhitreturnatthepromptforaguess |[45=1, ], key-intheechocommands |[45=1, ], key-theirinstancesareprivateto |[43=1, ], key-goodprogrammerscloselytracethroughtheircodeindetail |[39=1, ], key-guessesbeforetheprogramterminates |[39=1, ], key-pluginsintegratedintoyourfavoriteide |[39=1, ], key-inafuturelecturewewilllookatmoreapproachestotesting |[39=1, ], key-assumingyouhavenoerrorsinyourmakefile |[35=1, ], key-andoftenadeveloperwillupdatetherealtargetsanddependenciespartofthe |[35=1, ], key-wikipedia |[35=1, ], key-yes |[35=1, 42=1, ], key-insteadof |[33=1, 41=1, 43=1, 45=1, 47=1, ], key-cached |[33=2, ], key-youalwaysneedalocalrepositorytoworkwithgit |[33=1, ], key-notatree |[31=1, ], key-withsmallexperiments |[30=1, ], key-len |[29=1, 36=1, 41=4, ], key-seta |[27=15, ], key-treefind |[26=1, ], key-evenshowsnippetsofcodeonthewhiteboard |[19=1, ], key-onceincanvas |[19=1, ], key-brewupgrade |[17=1, ], key-llsharethemwitheveryone |[16=1, ], key-ifyouentertheaboveurlintoyourbrowser |[14=1, ], key-weathercode |[14=1, ], key-intheprecedingexamplessomevalueswereprintedasintegersandsomeasdecimalfractions |[14=1, ], key-donotputanytselogicinyourdata |[11=1, ], key-lengthofstrings |[10=1, ], key-include |[10=3, 14=2, 18=1, 36=3, 40=1, 41=15, 42=12, 43=8, ], key-wecannowrunasyncset |[3=1, ], key-forkmeongithubgimmick |[3=1, ], key-pagetitle |[3=5, ], key-pathlinks |[3=1, ], key-gimmick |[3=17, ], key-msg |[3=1, ], key-returntext |[3=2, ], key-wgaawje |[3=1, ], key-bghovpyh |[3=1, ], key-transposed |[3=1, ], key-breakcasecatchclassdefcontinueelseelseifendenumeratedeventsforfunctionglobalifmethodsotherwiseparforpersistentpropertiesreturnspmdswitchtrywhile |[3=1, ], key-visible |[3=145, ], key-cae |[3=2, ], ]
   1: [key-rlistthedirectoryanditssubdirectoriesrecursively |[47=1, ], key-printtheargumentsfordebugging |[45=1, ], key-usingthe |[45=1, ], key-isdefinedas |[43=1, ], key-thevariableisaconstant |[43=1, ], key-pleaseenteralineoftext |[42=1, ], key-sometimesyouneedtounderstandthatanarraynameis |[41=1, ], key-acoupleofcoolthingstonoteabout |[39=1, ], key-andsoon |[39=1, 45=1, ], key-although |[36=1, ], key-defineswheretheutilitylibraryismaintained |[35=1, ], key-thefirsttime |[34=1, ], key-thedetailsofwhichbeyondthescopeofthesenotes |[33=1, ], key-named |[33=1, ], key-highlydistributedsystems |[31=1, ], key-consistofthreesubsystems |[30=1, ], key-secondtoforwardapacketfromitsinputradiotoitsoutputradio |[30=1, ], key-sasimplefunctiontoprintthoselines |[29=1, ], key-putabstractionsincode |[28=1, ], key-purposemodulesfromlab |[28=1, ], key-setbisunchangedbutsetashouldhaveallitemsfrombothsets |[27=1, ], key-purposeiteratorforeachmodule |[27=1, ], key-andwritinggoodcode |[26=2, ], key-accessibility |[19=1, ], key-willbereported |[19=2, ], key-suchas |[18=1, 29=1, 30=2, 33=1, 39=1, 43=2, 46=1, ], key-unixbooks |[16=1, ], key-helper |[14=1, 27=4, ], key-thatyoushouldusewith |[14=1, ], key-optionsbesureyouhaveareasonableand |[14=1, ], key-thisallowsyouto |[13=1, ], key-itstartsempty |[13=1, ], key-writeanew |[11=1, ], key-aftersemicolonsinaforloop |[10=1, ], key-theendgoalistogeneratepublishableresultsbytheendoftheterm |[4=1, ], key-foreachtopic |[4=1, ], key-varhint |[3=1, ], key-rrandcontributors |[3=1, ], key-example |[3=1, 13=1, 18=5, 26=1, 33=1, 34=3, 36=30, 42=8, 43=6, 45=15, 46=4, 47=1, ], key-usevanillajs |[3=1, ], key-functionregisterbuildnavigation |[3=1, ], key-mangle |[3=1, ], key-mm |[3=1, ], key-iakoo |[3=1, ], key-removedata |[3=6, ], key-show |[3=27, 18=1, 33=1, 39=1, ], key-glyphiconshalflings |[3=2, ], key-eee |[3=13, ], ]


   ###Testing the indexer in shell script
   [flume:indexer] 120) ./indextestShell ../data 1 2

	1 and 2 didn't exit on compare
	Indexer works!!!!


   ###Testing Memory Leaks
   [flume:indexer] 208) valgrind --leak-check=full --show-reachable=yes ./indextest  COOL f
==17924== Memcheck, a memory error detector
==17924== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==17924== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==17924== Command: ./indextest COOL f
==17924== 
==17924== 
==17924== HEAP SUMMARY:
==17924==     in use at exit: 0 bytes in 0 blocks
==17924==   total heap usage: 171,101 allocs, 171,101 frees, 5,587,968 bytes allocated
==17924== 
==17924== All heap blocks were freed -- no leaks are possible
==17924== 
==17924== For counts of detected and suppressed errors, rerun with: -v
==17924== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

[flume:indexer] 209) valgrind --leak-check=full --show-reachable=yes ./indexer ../data COOL
==17929== Memcheck, a memory error detector
==17929== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==17929== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==17929== Command: ./indexer COOL
==17929== 
ERROR: Please use 2 paramters to the indexer command.
==17929== 
==17929== HEAP SUMMARY:
==17929==     in use at exit: 0 bytes in 0 blocks
==17929==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==17929== 
==17929== All heap blocks were freed -- no leaks are possible
==17929== 
==17929== For counts of detected and suppressed errors, rerun with: -v
==17929== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

