﻿#define STB_RECT_PACK_IMPLEMENTATION
#include "ArgonGui.h"
#include <fstream>

static const int8_t proggy_clean_ttf_compressed_data_base85[11980 + 1] =
"7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
"2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
"`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
"i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
"kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
"*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
"tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
"ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
"x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
"CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
"U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
"'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
"_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gU"
"Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
"/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
"%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
"OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
"h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
"o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
"j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
"sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
"eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
"M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
"LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
"%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
"Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
"a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
"$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
"nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
"7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
"D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
"P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
"bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
"h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
"V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
"sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
"$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
"hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]U"
"@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
"w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
"u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
"d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
"6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
"b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
"tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
"$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
"7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
"u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
"LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
"_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
"hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
"^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
"+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
"9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
"CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
"hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
"8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
"S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
"0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
"+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
"M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
"?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
"Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
"[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
"wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
"Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
"MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
"i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
"1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
"iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
"URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
"w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
"d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
"A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
"/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
"m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
"TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
"GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
"O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

static uint32_t stb_decompress_length(const unsigned char* input)
{
	return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char* stb__barrier_out_e, * stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;
static void stb__match(const unsigned char* data, uint32_t length)
{
	// INVERSE of memmove... write each byte before copying the next...
	if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
	if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e + 1; return; }
	while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char* data, uint32_t length)
{
	if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
	if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e + 1; return; }
	memcpy(stb__dout, data, length);
	stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char* stb_decompress_token(const unsigned char* i)
{
	if (*i >= 0x20) { // use fewer if's for cases that expand small
		if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
		else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
		else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
	}
	else { // more ifs for cases that expand large, since overhead is amortized
		if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
		else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
		else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
		else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
		else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
		else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
	}
	return i;
}

static uint32_t stb_adler32(uint32_t adler32, unsigned char* buffer, uint32_t buflen)
{
	const unsigned long ADLER_MOD = 65521;
	unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
	unsigned long blocklen = buflen % 5552;

	unsigned long i;
	while (buflen) {
		for (i = 0; i + 7 < blocklen; i += 8) {
			s1 += buffer[0], s2 += s1;
			s1 += buffer[1], s2 += s1;
			s1 += buffer[2], s2 += s1;
			s1 += buffer[3], s2 += s1;
			s1 += buffer[4], s2 += s1;
			s1 += buffer[5], s2 += s1;
			s1 += buffer[6], s2 += s1;
			s1 += buffer[7], s2 += s1;

			buffer += 8;
		}

		for (; i < blocklen; ++i)
			s1 += *buffer++, s2 += s1;

		s1 %= ADLER_MOD, s2 %= ADLER_MOD;
		buflen -= blocklen;
		blocklen = 5552;
	}
	return (uint32_t)(s2 << 16) + (uint32_t)s1;
}

static uint32_t stb_decompress(unsigned char* output, const unsigned char* i, uint32_t /*length*/)
{
	if (stb__in4(0) != 0x57bC0000) return 0;
	if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
	const uint32_t olen = stb_decompress_length(i);
	stb__barrier_in_b = i;
	stb__barrier_out_e = output + olen;
	stb__barrier_out_b = output;
	i += 16;

	stb__dout = output;
	for (;;) {
		const unsigned char* old_i = i;
		i = stb_decompress_token(i);
		if (i == old_i) {
			if (*i == 0x05 && i[1] == 0xfa) {
				if (stb__dout != output + olen) return 0;
				if (stb_adler32(1, output, olen) != (uint32_t)stb__in4(2))
					return 0;
				return olen;
			}
			else {
				return 0;
			}
		}
		if (stb__dout > output + olen)
			return 0;
	}
}
static uint32_t Decode85Byte(char c) { return c >= '\\' ? c - 36 : c - 35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
	while (*src)
	{
		uint32_t tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
		dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
		src += 5;
		dst += 4;
	}
}

ArRenderListSharedData::ArRenderListSharedData()
{
	circleSegmentMaxError = 0.3f;
	for (int i = 0; i < arcFastVtx.size(); i++)
	{
		const float a = ((float)i * 2 * AR_PI) / (float)arcFastVtx.size();
		arcFastVtx[i] = ArVec2(cosf(a), sinf(a));
	}
	for (int i = 0; i < circleSegmentCounts.size(); i++)
	{
		const float radius = (float)i;
		circleSegmentCounts[i] = (uint8_t)((i > 0) ? ArClamp((((((int)ceilf(3.14159265358979323846f / acosf(1 - ArMin((circleSegmentMaxError), (radius)) / (radius)))) + 1) / 2) * 2), 4, 512) : 48);
	}
	arcFastRadiusCutoff = ((circleSegmentMaxError) / (1 - cosf(3.14159265358979323846f / ArMax((float)(48), 3.14159265358979323846f))));
}

ArRenderList::ArRenderList(ArRenderListSharedData* sharedData)
{
	this->sharedData = sharedData;
	currentTexture = sharedData->renderManager->GetDefaultTexture();
	AddNewBatch();
}

ArRenderList::~ArRenderList()
{
	textureStack.clear();
	scissorStack.clear();
	customPixelShaderStack.clear();
	customVertexShaderStack.clear();
	batches.clear();
	vertices.clear();
	indices.clear();
}

void ArRenderList::AddLine(const ArVec2& from, const ArVec2& to, uint32_t color, float thickness)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathLineTo(from);
	PathLineTo(to);
	PathStroke(color, ArRenderFlag::None, thickness);
}

void ArRenderList::AddRect(const ArRect& rect, uint32_t color, float thickness, ArVec4 rounding, ArRenderFlag flags)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathRect(ArRect(rect.min + ArVec2(0.50f, 0.50f), rect.max - ArVec2(0.50f, 0.50f)), rounding);
	PathStroke(color, ArRenderFlag::Closed, thickness);
}

void ArRenderList::AddRectFilled(const ArRect& rect, uint32_t color, ArVec4 rounding, ArRenderFlag flags)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathRect(rect, rounding);
	PathFillConvex(color);
}

void ArRenderList::AddRectFilledMultiColor(const ArRect& rect, uint32_t colorUpperLeft, uint32_t colorUpperRight, uint32_t colorBottomRight, uint32_t colorBottomLeft)
{
	if (((colorUpperLeft | colorUpperRight | colorBottomRight | colorBottomLeft) & AR_COL32_A_MASK) == 0)
		return;

	PrimRectWithCornerColor(rect, sharedData->uvOfWhitInTexture, colorUpperLeft, colorUpperRight, colorBottomRight, colorBottomLeft);
}

void ArRenderList::AddQuad(const ArVec2& a, const ArVec2& b, const ArVec2& c, const ArVec2& d, uint32_t color, float thickness)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathStroke(color, ArRenderFlag::Closed, thickness);
}

void ArRenderList::AddQuadFilled(const ArVec2& a, const ArVec2& b, const ArVec2& c, const ArVec2& d, uint32_t color)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathFillConvex(color);
}

void ArRenderList::AddTriangle(const ArVec2& a, const ArVec2& b, const ArVec2& c, uint32_t color)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathStroke(color, ArRenderFlag::Closed);
}

void ArRenderList::AddTriangleFilled(const ArVec2& a, const ArVec2& b, const ArVec2& c, uint32_t color)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathFillConvex(color);
}

void ArRenderList::AddCircle(const ArVec2& center, float radius, uint32_t color, float thickness, int numSegments)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	if (numSegments <= 0)
	{
		PathArcToFast(center, radius - 0.5f, 0, 48, 0);
	}
	else
	{
		numSegments = ArClamp(numSegments, 3, 512);

		const float a_max = (AR_PI * 2.0f) * ((float)numSegments - 1.f) / (float)numSegments;
		PathArcTo(center, radius - 0.5f, 0.0f, a_max, numSegments);
	}
	PathStroke(color, ArRenderFlag::Closed, thickness);
}

void ArRenderList::AddCircleFilled(const ArVec2& center, float radius, uint32_t color, int numSegments)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	if (numSegments <= 0)
	{
		PathArcToFast(center, radius - 0.5f, 0, 48);
	}
	else
	{
		numSegments = ArClamp(numSegments, 3, 512);
		const float a_max = AR_PI * 2.0f * ((float)numSegments - 1.f) / (float)numSegments;
		PathArcTo(center, radius - 0.5f, 0.0f, a_max, numSegments);
	}
	PathFillConvex(color);
}

void ArRenderList::AddImage(ArTextureID texture, const ArRect& rect, const ArRect& uv, uint32_t color)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	if (texture != currentTexture)
		PushTexture(texture);
	PrimRectWithUV(rect, uv, color);
	if (texture != currentTexture)
		PopTexture();
}

void ArRenderList::AddImage(const ArImage& image, const ArRect& rect, uint32_t color)
{
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	if (image.texture != currentTexture)
		PushTexture(image.texture);
	PrimRectWithUV(rect, image.uv, color);
	if (image.texture != currentTexture)
		PopTexture();
}

void ArRenderList::AddPolyline(const std::vector<ArVec2>& points, uint32_t color, ArRenderFlag flags, float thickness)
{
	if (points.size() < 2 || (color & AR_COL32_A_MASK) == 0)
		return;

	const bool closed = ArHasFlag(flags, ArRenderFlag::Closed);
	const bool useAA = ArHasFlag(listFlags, ArRenderListFlag::UseAntiAliasing);
	const ArVec2 opaqueUV = sharedData->uvOfWhitInTexture;
	const size_t count = closed ? points.size() : points.size() - 1; // The number of line segments we need to draw
	const bool isThickLine = (thickness > 1.f);

	if (useAA)
	{
		const uint32_t colorAlpha = color & ~AR_COL32_A_MASK;

		thickness = ArMax(thickness, 1.f);
		const int integerThickness = (int)thickness;
		const float fractionalThickness = thickness - (float)integerThickness;

		const bool useTexture = integerThickness < 63 && fractionalThickness <= 0.00001f;

		const size_t indexCount = useTexture ? (count * 6) : (isThickLine ? count * 18 : count * 12);
		const size_t vertexCount = useTexture ? (points.size() * 2) : (isThickLine ? points.size() * 4 : points.size() * 3);
		RequireSpace(vertexCount, indexCount);

		// Temporary buffer
		// The first <points.size()> items are normals at each line point, then after that there are either 2 or 4 temp points for each line point
		std::vector<ArVec2> tempNormals(points.size());
		std::vector<ArVec2> tempPoints(points.size() * (useTexture || !isThickLine ? 2 : 4));

		// Calculate normals (tangents) for each line segment
		for (size_t i1 = 0; i1 < count; i1++)
		{
			const size_t i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
			float dx = points[i2].x - points[i1].x;
			float dy = points[i2].y - points[i1].y;
			{
				float d2 = dx * dx + dy * dy; if (d2 > 0.0f) {
					float inv_len = ArRsqrt(d2); dx *= inv_len; dy *= inv_len;
				}
			} (void)0;
			tempNormals[i1].x = dy;
			tempNormals[i1].y = -dx;
		}
		if (!closed)
			tempNormals[points.size() - 1] = tempNormals[points.size() - 2];

		if (useTexture || !isThickLine)
		{
			// [PATH 1] Texture-based lines (thick or non-thick)
			// [PATH 2] Non texture-based lines (non-thick)

			const float halfDrawSize = useTexture ? ((thickness * 0.5f) + 1) : 1.f;

			// If line is not closed, the first and last points need to be generated differently as there are no normals to blend
			if (!closed)
			{
				tempPoints[0] = points[0] + tempNormals[0] * halfDrawSize;
				tempPoints[1] = points[0] - tempNormals[0] * halfDrawSize;
				tempPoints[(points.size() - 1) * 2 + 0] = points[points.size() - 1] + tempNormals[points.size() - 1] * halfDrawSize;
				tempPoints[(points.size() - 1) * 2 + 1] = points[points.size() - 1] - tempNormals[points.size() - 1] * halfDrawSize;
			}

			// Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
			uint32_t idx1 = (uint32_t)vertices.size(); // Vertex index for start of line segment
			for (size_t i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
			{
				const size_t i2 = (i1 + 1) == points.size() ? 0 : (i1 + 1); // i2 is the second point of the line segment
				const uint32_t idx2 = ((i1 + 1) == points.size()) ? (uint32_t)vertices.size() : (idx1 + (useTexture ? 2 : 3)); // Vertex index for end of segment

				// Average normals
				float dm_x = (tempNormals[i1].x + tempNormals[i2].x) * 0.5f;
				float dm_y = (tempNormals[i1].y + tempNormals[i2].y) * 0.5f;
				float d2 = dm_x * dm_x + dm_y * dm_y; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; dm_x *= inv_lensq; dm_y *= inv_lensq;
				dm_x *= halfDrawSize; // dm_x, dm_y are offset to the outer edge of
				dm_y *= halfDrawSize;

				// Add temporary vertexes for the outer edges
				ArVec2* out_vtx = &tempPoints[i2 * 2];
				out_vtx[0].x = points[i2].x + dm_x;
				out_vtx[0].y = points[i2].y + dm_y;
				out_vtx[1].x = points[i2].x - dm_x;
				out_vtx[1].y = points[i2].y - dm_y;

				if (useTexture)
				{
					// Add indices for two triangles
					indices.emplace_back((uint16_t)(idx2 + 0));
					indices.emplace_back((uint16_t)(idx1 + 0));
					indices.emplace_back((uint16_t)(idx1 + 1)); // Right tri
					indices.emplace_back((uint16_t)(idx2 + 1));
					indices.emplace_back((uint16_t)(idx1 + 1));
					indices.emplace_back((uint16_t)(idx2 + 0)); // Left tri
				}
				else
				{
					// Add indexes for four triangles
					indices.emplace_back((uint16_t)(idx2 + 0));
					indices.emplace_back((uint16_t)(idx1 + 0));
					indices.emplace_back((uint16_t)(idx1 + 2)); // Right tri 1
					indices.emplace_back((uint16_t)(idx1 + 2));
					indices.emplace_back((uint16_t)(idx2 + 2));
					indices.emplace_back((uint16_t)(idx2 + 0)); // Right tri 2
					indices.emplace_back((uint16_t)(idx2 + 1));
					indices.emplace_back((uint16_t)(idx1 + 1));
					indices.emplace_back((uint16_t)(idx1 + 0)); // Left tri 1
					indices.emplace_back((uint16_t)(idx1 + 0));
					indices.emplace_back((uint16_t)(idx2 + 0));
					indices.emplace_back((uint16_t)(idx2 + 1)); // Left tri 2
				}

				idx1 = idx2;
			}

			if (useTexture)
			{
				ArVec4 tex_uvs = sharedData->uvOfBakeLines[integerThickness];
				const ArVec2 texUV0(tex_uvs.x, tex_uvs.y);
				const ArVec2 texUV1(tex_uvs.z, tex_uvs.w);
				for (size_t i = 0; i < points.size(); i++)
				{
					vertices.emplace_back(tempPoints[i * 2 + 0], texUV0, color);
					vertices.emplace_back(tempPoints[i * 2 + 1], texUV1, color);
				}
			}
			else
			{
				// If we're not using a texture, we need the center vertex as well
				for (size_t i = 0; i < points.size(); i++)
				{
					vertices.emplace_back(points[i], opaqueUV, color); // Center of line
					vertices.emplace_back(tempPoints[i * 2 + 0], opaqueUV, colorAlpha);
					vertices.emplace_back(tempPoints[i * 2 + 1], opaqueUV, colorAlpha);
				}
			}
		}
		else
		{
			const float halfInnerThickness = (thickness - 1.f) * 0.5f;

			if (!closed)
			{
				const size_t pointsLast = points.size() - 1;
				tempPoints[0] = points[0] + tempNormals[0] * (halfInnerThickness + 1.f);
				tempPoints[1] = points[0] + tempNormals[0] * halfInnerThickness;
				tempPoints[2] = points[0] - tempNormals[0] * halfInnerThickness;
				tempPoints[3] = points[0] - tempNormals[0] * (halfInnerThickness + 1.f);
				tempPoints[pointsLast * 4 + 0] = points[pointsLast] + tempNormals[pointsLast] * (halfInnerThickness + 1.f);
				tempPoints[pointsLast * 4 + 1] = points[pointsLast] + tempNormals[pointsLast] * halfInnerThickness;
				tempPoints[pointsLast * 4 + 2] = points[pointsLast] - tempNormals[pointsLast] * halfInnerThickness;
				tempPoints[pointsLast * 4 + 3] = points[pointsLast] - tempNormals[pointsLast] * (halfInnerThickness + 1.f);
			}

			uint32_t idx1 = (uint32_t)vertices.size(); // Vertex index for start of line segment
			for (size_t i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
			{
				const size_t i2 = (i1 + 1) == points.size() ? 0 : (i1 + 1); // i2 is the second point of the line segment
				const uint32_t idx2 = (i1 + 1) == points.size() ? (uint32_t)vertices.size() : (idx1 + 4); // Vertex index for end of segment

				// Average normals
				float dm_x = (tempNormals[i1].x + tempNormals[i2].x) * 0.5f;
				float dm_y = (tempNormals[i1].y + tempNormals[i2].y) * 0.5f;
				float d2 = dm_x * dm_x + dm_y * dm_y; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; dm_x *= inv_lensq; dm_y *= inv_lensq;
				float dm_out_x = dm_x * (halfInnerThickness + 1.f);
				float dm_out_y = dm_y * (halfInnerThickness + 1.f);
				float dm_in_x = dm_x * halfInnerThickness;
				float dm_in_y = dm_y * halfInnerThickness;

				// Add temporary vertices
				ArVec2* out_vtx = &tempPoints[i2 * 4];
				out_vtx[0].x = points[i2].x + dm_out_x;
				out_vtx[0].y = points[i2].y + dm_out_y;
				out_vtx[1].x = points[i2].x + dm_in_x;
				out_vtx[1].y = points[i2].y + dm_in_y;
				out_vtx[2].x = points[i2].x - dm_in_x;
				out_vtx[2].y = points[i2].y - dm_in_y;
				out_vtx[3].x = points[i2].x - dm_out_x;
				out_vtx[3].y = points[i2].y - dm_out_y;

				// Add indexes
				indices.emplace_back((uint16_t)(idx2 + 1));
				indices.emplace_back((uint16_t)(idx1 + 1));
				indices.emplace_back((uint16_t)(idx1 + 2));
				indices.emplace_back((uint16_t)(idx1 + 2));
				indices.emplace_back((uint16_t)(idx2 + 2));
				indices.emplace_back((uint16_t)(idx2 + 1)); // Right tri 1
				indices.emplace_back((uint16_t)(idx2 + 1));
				indices.emplace_back((uint16_t)(idx1 + 1));
				indices.emplace_back((uint16_t)(idx1 + 0));
				indices.emplace_back((uint16_t)(idx1 + 0));
				indices.emplace_back((uint16_t)(idx2 + 0));
				indices.emplace_back((uint16_t)(idx2 + 1)); // Left tri 1
				indices.emplace_back((uint16_t)(idx2 + 2));
				indices.emplace_back((uint16_t)(idx1 + 2));
				indices.emplace_back((uint16_t)(idx1 + 3));
				indices.emplace_back((uint16_t)(idx1 + 3));
				indices.emplace_back((uint16_t)(idx2 + 3));
				indices.emplace_back((uint16_t)(idx2 + 2)); // Right tri 2

				idx1 = idx2;
			}

			// Add vertices
			for (size_t i = 0; i < points.size(); i++)
			{
				vertices.emplace_back(tempPoints[i * 4 + 0], opaqueUV, colorAlpha);
				vertices.emplace_back(tempPoints[i * 4 + 1], opaqueUV, color);
				vertices.emplace_back(tempPoints[i * 4 + 2], opaqueUV, color);
				vertices.emplace_back(tempPoints[i * 4 + 3], opaqueUV, colorAlpha);
			}
		}
	}
	else
	{
		const size_t indexCount = count * 6;
		const size_t vertexCount = count * 4;
		RequireSpace(vertexCount, indexCount);

		for (int i1 = 0; i1 < count; i1++)
		{
			const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
			const ArVec2& p1 = points[i1];
			const ArVec2& p2 = points[i2];

			ArVec2 diff = p2 - p1;
			float d2 = diff.x * diff.x + diff.y * diff.y;
			if (d2 > 0.0f)
			{
				float inv_length = ArRsqrt(d2);
				diff = diff * inv_length;
			}
			diff = diff * (thickness * 0.5f);

			indices.emplace_back((uint16_t)(vertices.size() + 0));
			indices.emplace_back((uint16_t)(vertices.size() + 1));
			indices.emplace_back((uint16_t)(vertices.size() + 2));
			indices.emplace_back((uint16_t)(vertices.size() + 0));
			indices.emplace_back((uint16_t)(vertices.size() + 2));
			indices.emplace_back((uint16_t)(vertices.size() + 3));

			vertices.emplace_back(ArVec2(p1.x + diff.y, p1.y - diff.x), opaqueUV, color);
			vertices.emplace_back(ArVec2(p2.x + diff.y, p2.y - diff.x), opaqueUV, color);
			vertices.emplace_back(ArVec2(p2.x - diff.y, p2.y + diff.x), opaqueUV, color);
			vertices.emplace_back(ArVec2(p1.x - diff.y, p1.y + diff.x), opaqueUV, color);
		}
	}
}

void ArRenderList::AddConvexPolyFilled(const std::vector<ArVec2>& points, uint32_t color)
{
	if (points.size() < 3 || (color & AR_COL32_A_MASK) == 0)
		return;
	const bool useAA = ArHasFlag(listFlags, ArRenderListFlag::UseAntiAliasing);
	const ArVec2 opaqueUV = sharedData->uvOfWhitInTexture;

	if (useAA)
	{
		// copy from imgui
		const uint32_t colorAlpha = color & ~AR_COL32_A_MASK;
		const size_t indexCount = (points.size() - 2) * 3 + points.size() * 6;
		const size_t vertexCount = points.size() * 2;
		RequireSpace(vertexCount, indexCount);

		uint32_t vertexInnerIdx = (uint32_t)vertices.size();
		uint32_t vertexOuterIdx = vertexInnerIdx + 1;
		for (size_t i = 2; i < points.size(); i++)
		{
			indices.emplace_back((uint16_t)(vertexInnerIdx));
			indices.emplace_back((uint16_t)(vertexInnerIdx + ((i - 1) << 1)));
			indices.emplace_back((uint16_t)(vertexInnerIdx + (i << 1)));
		}

		// Compute normals
		std::vector<ArVec2> tempNormals(points.size());
		for (size_t i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++)
		{
			const ArVec2& p0 = points[i0];
			const ArVec2& p1 = points[i1];
			float dx = p1.x - p0.x;
			float dy = p1.y - p0.y;
			{
				float d2 = dx * dx + dy * dy; if (d2 > 0.0f) {
					float inv_len = ArRsqrt(d2); dx *= inv_len; dy *= inv_len;
				}
			} (void)0;
			tempNormals[i0].x = dy;
			tempNormals[i0].y = -dx;
		}

		for (size_t i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++)
		{
			// Average normals
			const ArVec2& n0 = tempNormals[i0];
			const ArVec2& n1 = tempNormals[i1];
			float dm_x = (n0.x + n1.x) * 0.5f;
			float dm_y = (n0.y + n1.y) * 0.5f;
			{
				float d2 = dm_x * dm_x + dm_y * dm_y; if (d2 > 0.0f) {
					float inv_len = ArRsqrt(d2); dm_x *= inv_len; dm_y *= inv_len;
				}
			}
			dm_x *= 0.5f;
			dm_y *= 0.5f;
			// Add indexes for fringes
			indices.emplace_back((uint16_t)(vertexInnerIdx + (i1 << 1)));
			indices.emplace_back((uint16_t)(vertexInnerIdx + (i0 << 1)));
			indices.emplace_back((uint16_t)(vertexOuterIdx + (i0 << 1)));
			indices.emplace_back((uint16_t)(vertexOuterIdx + (i0 << 1)));
			indices.emplace_back((uint16_t)(vertexOuterIdx + (i1 << 1)));
			indices.emplace_back((uint16_t)(vertexInnerIdx + (i1 << 1)));
			// Add vertices
			vertices.emplace_back(ArVec2(points[i1].x - dm_x, points[i1].y - dm_y), opaqueUV, color);
			vertices.emplace_back(ArVec2(points[i1].x + dm_x, points[i1].y + dm_y), opaqueUV, colorAlpha);
		}
	}
	else
	{
		const size_t indexCount = (points.size() - 2) * 3;
		const size_t vertexCount = points.size();
		RequireSpace(vertexCount, indexCount);

		for (size_t i = 2; i < points.size(); i++)
		{
			indices.emplace_back((uint16_t)(vertices.size() + 0));
			indices.emplace_back((uint16_t)(vertices.size() + i - 1));
			indices.emplace_back((uint16_t)(vertices.size() + i));
		}
		for (size_t i = 0; i < points.size(); i++)
		{
			vertices.emplace_back(points[i], opaqueUV, color);
		}
	}
}

void ArRenderList::AddText(ArStringView text, uint32_t size, const ArVec2& pos, uint32_t color)
{
	if (text.empty())
		return;
	if ((color & AR_COL32_A_MASK) == 0)
		return;

	const std::vector<ArGlyphInfo*>& glyphs = sharedData->renderManager->TryGetGlyphs(text, size);

	float x = pos.x;
	for (const auto& glyph : glyphs)
	{
		if (glyph == nullptr)
			continue;

		if (glyph->visible)
		{
			const ArTextureLand& land = sharedData->renderManager->lands[glyph->landIndex];

			PushTexture(land.GetTexture());

			const ArVec2 glyphPos = ArVec2(x, pos.y) + glyph->min;
			if (glyph->colored)
				color |= ~AR_COL32_A_MASK;
			PrimRectWithUV(ArRect(glyphPos, glyphPos + glyph->size), glyph->uv, color);

			if (glyph->colored)
				color &= AR_COL32_A_MASK;
		}

		x += glyph->advanceX;
	}
}

void ArRenderList::AddNewBatch()
{
	ArRenderBatch batch = ArRenderBatch();
	batch.scissor = currentScissor;
	batch.indexOffset = indices.size();
	batch.texture = currentTexture;
	batches.push_back(batch);
	currentBatch = &batches.back();
}

void ArRenderList::PrimRectWithUV(const ArRect& rect, const ArRect& uv, uint32_t color)
{
	RequireSpace(4, 6);
	indices.emplace_back((uint16_t)(vertices.size() + 0));
	indices.emplace_back((uint16_t)(vertices.size() + 1));
	indices.emplace_back((uint16_t)(vertices.size() + 2));
	indices.emplace_back((uint16_t)(vertices.size() + 0));
	indices.emplace_back((uint16_t)(vertices.size() + 2));
	indices.emplace_back((uint16_t)(vertices.size() + 3));
	vertices.emplace_back(ArVertex{ rect.min, uv.min, color });
	vertices.emplace_back(ArVertex{ ArVec2(rect.max.x, rect.min.y), ArVec2(uv.max.x, uv.min.y), color });
	vertices.emplace_back(ArVertex{ rect.max, uv.max, color });
	vertices.emplace_back(ArVertex{ ArVec2(rect.min.x, rect.max.y), ArVec2(uv.min.x, uv.max.y), color });
}

void ArRenderList::PrimRectWithCornerColor(const ArRect& rect, const ArVec2& uv, uint32_t colorUpperLeft, uint32_t colorUpperRight, uint32_t colorBottomRight, uint32_t colorBottomLeft)
{
	RequireSpace(4, 6);
	indices.emplace_back((uint16_t)(vertices.size() + 0));
	indices.emplace_back((uint16_t)(vertices.size() + 1));
	indices.emplace_back((uint16_t)(vertices.size() + 2));
	indices.emplace_back((uint16_t)(vertices.size() + 0));
	indices.emplace_back((uint16_t)(vertices.size() + 2));
	indices.emplace_back((uint16_t)(vertices.size() + 3));
	vertices.emplace_back(ArVertex{ ArVec2(rect.min.x, rect.min.y), uv, colorUpperLeft });
	vertices.emplace_back(ArVertex{ ArVec2(rect.max.x, rect.min.y), uv, colorUpperRight });
	vertices.emplace_back(ArVertex{ ArVec2(rect.max.x, rect.max.y), uv, colorBottomRight });
	vertices.emplace_back(ArVertex{ ArVec2(rect.min.x, rect.max.y), uv, colorBottomLeft });
}

void ArRenderList::PathRect(const ArRect& rect, ArVec4 rounding)
{
	const ArVec2 a = ArVec2(rect.min.x, rect.min.y);
	const ArVec2 b = ArVec2(rect.max.x, rect.min.y);
	const ArVec2 c = ArVec2(rect.max.x, rect.max.y);
	const ArVec2 d = ArVec2(rect.min.x, rect.max.y);

	if (rounding.x <= 0.f && rounding.y <= 0.f && rounding.z <= 0.f && rounding.w <= 0.f)
	{
		PathLineTo(a);
		PathLineTo(b);
		PathLineTo(c);
		PathLineTo(d);
	}
	else
	{
		const float rounding_tl = ArMax(0.f, rounding.x);
		const float rounding_tr = ArMax(0.f, rounding.y);
		const float rounding_br = ArMax(0.f, rounding.z);
		const float rounding_bl = ArMax(0.f, rounding.w);
		PathArcToFast(ArVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
		PathArcToFast(ArVec2(c.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
		PathArcToFast(ArVec2(c.x - rounding_br, c.y - rounding_br), rounding_br, 0, 3);
		PathArcToFast(ArVec2(a.x + rounding_bl, c.y - rounding_bl), rounding_bl, 3, 6);
	}
}

void ArRenderList::PathArcTo(const ArVec2& center, float radius, float aMin, float aMax, int numSegments)
{
	if (radius <= 0.5f)
	{
		path.push_back(center);
		return;
	}

	if (numSegments > 0)
	{
		PathArcToN(center, radius, aMin, aMax, numSegments);
		return;
	}

	if (radius <= sharedData->arcFastRadiusCutoff)
	{
		const bool a_is_reverse = aMax < aMin;

		// We are going to use precomputed values for mid samples.
		// Determine first and last sample in lookup table that belong to the arc.
		const float a_min_sample_f = 48 * aMin / (AR_PI * 2.0f);
		const float a_max_sample_f = 48 * aMax / (AR_PI * 2.0f);

		const int a_min_sample = a_is_reverse ? (int)ArFloorSigned(a_min_sample_f) : (int)ceilf(a_min_sample_f);
		const int a_max_sample = a_is_reverse ? (int)ceilf(a_max_sample_f) : (int)ArFloorSigned(a_max_sample_f);
		const int a_mid_samples = a_is_reverse ? ArMax(a_min_sample - a_max_sample, 0) : ArMax(a_max_sample - a_min_sample, 0);

		const float a_min_segment_angle = a_min_sample * AR_PI * 2.0f / 48;
		const float a_max_segment_angle = a_max_sample * AR_PI * 2.0f / 48;
		const bool a_emit_start = fabsf(a_min_segment_angle - aMin) >= 1e-5f;
		const bool a_emit_end = fabsf(aMax - a_max_segment_angle) >= 1e-5f;

		path.reserve(path.size() + (a_mid_samples + 1 + (a_emit_start ? 1 : 0) + (a_emit_end ? 1 : 0)));
		if (a_emit_start)
			path.push_back(ArVec2(center.x + cosf(aMin) * radius, center.y + sinf(aMin) * radius));
		if (a_mid_samples > 0)
			PathArcToFast(center, radius, a_min_sample, a_max_sample, 0);
		if (a_emit_end)
			path.push_back(ArVec2(center.x + cosf(aMax) * radius, center.y + sinf(aMax) * radius));
	}
	else
	{
		const float arc_length = fabsf(aMax - aMin);
		const int circle_segment_count = CalcCircleAutoSegmentCount(radius);
		const int arc_segment_count = ArMax((int)ceilf(circle_segment_count * arc_length / (AR_PI * 2.0f)), (int)(2.0f * AR_PI / arc_length));
		PathArcToN(center, radius, aMin, aMax, arc_segment_count);
	}
}

void ArRenderList::PathArcToN(const ArVec2& center, float radius, float aMin, float aMax, int numSegments)
{
	if (radius <= 0.f)
	{
		path.push_back(center);
		return;
	}

	path.reserve(path.size() + (numSegments + 1));
	for (int i = 0; i <= numSegments; i++)
	{
		const float a = aMin + ((float)i / (float)numSegments) * (aMax - aMin);
		path.emplace_back(center.x + cosf(a) * radius, center.y + sinf(a) * radius);
	}
}

void ArRenderList::PathArcToFast(const ArVec2& center, float radius, int aMinSample, int aMaxSample, int aStep)
{
	if (radius <= 0.f)
	{
		path.push_back(center);
		return;
	}

	if (aStep <= 0)
		aStep = 48 / CalcCircleAutoSegmentCount(radius);

	const int sampleRange = (int)fabsf((float)(aMaxSample - aMinSample));
	const int aNextStep = aStep;

	int samples = sampleRange + 1;
	bool extraMaxSample = false;
	if (aStep > 1)
	{
		samples = sampleRange / aStep + 1;
		const int overstep = sampleRange % aStep;

		if (overstep > 0)
		{
			extraMaxSample = true;
			samples++;

			if (sampleRange > 0)
				aStep -= (aStep - overstep) / 2;
		}
	}

	path.reserve(path.size() + samples);

	int sampleIndex = aMinSample;
	if (sampleIndex < 0 || sampleIndex >= 48)
	{
		sampleIndex = sampleIndex % 48;
		if (sampleIndex < 0)
			sampleIndex += 48;
	}

	if (aMaxSample >= aMinSample)
	{
		for (int a = aMinSample; a <= aMaxSample; a += aStep, sampleIndex += aStep, aStep = aNextStep)
		{
			if (sampleIndex >= 48)
				sampleIndex -= 48;

			path.emplace_back(center.x + sharedData->arcFastVtx[sampleIndex].x * radius, center.y + sharedData->arcFastVtx[sampleIndex].y * radius);
		}
	}
	else
	{
		for (int a = aMinSample; a >= aMaxSample; a -= aStep, sampleIndex -= aStep, aStep = aNextStep)
		{
			if (sampleIndex < 0)
				sampleIndex += 48;

			path.emplace_back(center.x + sharedData->arcFastVtx[sampleIndex].x * radius, center.y + sharedData->arcFastVtx[sampleIndex].y * radius);
		}
	}

	if (extraMaxSample)
	{
		int normalizedMaxSample = aMaxSample % 48;
		if (normalizedMaxSample < 0)
			normalizedMaxSample += 48;

		path.emplace_back(center.x + sharedData->arcFastVtx[normalizedMaxSample].x * radius, center.y + sharedData->arcFastVtx[normalizedMaxSample].y * radius);
	}
}

void ArRenderList::PathArcToFast(const ArVec2& center, float radius, int aMinOf12, int aMaxOf12)
{
	if (radius <= 0.f)
	{
		path.push_back(center);
		return;
	}
	PathArcToFast(center, radius, aMinOf12 * 4, aMaxOf12 * 4, 0);
}

void ArRenderList::PushTexture(ArTextureID texture)
{
	currentTexture = texture;
	textureStack.push_back(texture);
	OnChangedTexture();
}

void ArRenderList::PushScissor(ArRect scissor)
{
	currentScissor = scissor;
	scissorStack.push_back(scissor);
	OnChangedScissor();
}

void ArRenderList::PopTexture()
{
	if (textureStack.empty())
		return;
	currentTexture = textureStack.empty() ? nullptr : textureStack.back();
	textureStack.pop_back();
	OnChangedTexture();
}

void ArRenderList::PopScissor()
{
	if (scissorStack.empty())
		return;
	currentScissor = scissorStack.empty() ? ArRect() : scissorStack.back();
	scissorStack.pop_back();
	OnChangedScissor();
}

void ArRenderList::RequireSpace(size_t vertexCount, size_t indexCount)
{
	currentBatch->indexCount += (uint32_t)indexCount;
}

void ArRenderList::OnChangedTexture()
{
	if (currentBatch->indexCount != 0 && currentBatch->texture != currentTexture)
	{
		AddNewBatch();
		return;
	}
	currentBatch->texture = currentTexture;
}

void ArRenderList::OnChangedScissor()
{
	if (currentBatch->indexCount != 0 && memcmp(&currentBatch->scissor, &currentScissor, sizeof(ArRect)) != 0 && currentBatch->scissor.GetArea() > 0.f)
	{
		AddNewBatch();
		return;
	}
	currentBatch->scissor = currentScissor;
}

int ArRenderList::CalcCircleAutoSegmentCount(float radius) const
{
	const int radius_idx = (int)(radius + 0.999999f); // ceil to never reduce accuracy
	if (radius_idx < sharedData->circleSegmentCounts.size())
		return sharedData->circleSegmentCounts[radius_idx]; // Use cached value
	else
		return ArClamp((((((int)ceilf(AR_PI / acosf(1 - ArMin((sharedData->circleSegmentMaxError), (radius)) / (radius)))) + 1) / 2) * 2), 4, 512);
}

void ArTextureLand::OnDestroy(IArgonRenderer* renderer)
{
	if (pixels)
	{
		delete[] pixels;
		pixels = nullptr;
	}
	dirty = false;
	renderer->ReleaseTexture(textureId);
}

ArGlyphInfo* ArFont::GetGlyphNoFallback(uint32_t codepoint, uint32_t size, ArGlyphFlag flags)
{
	ArGlyphInfo* glyphInfo = FindGlyph(codepoint, size, flags);
	if (glyphInfo)
		return glyphInfo;
	return nullptr;
}

ArGlyphInfo* ArFont::TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags)
{
	ArGlyphInfo* glyphInfo = FindGlyph(codepoint, size, flags);
	if (glyphInfo)
		return glyphInfo;
	return fallbackGlyph;
}

ArGlyphInfo* ArFont::FindGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags)
{
	GlyphCacheKey key = GlyphCacheKey(codepoint, size, flags);
	auto it = glyphs.find(key);
	if (it != glyphs.end())
		return &it->second;
	if (!glyphParser->HasGlyph(fontId, codepoint))
		return nullptr;
	auto parserResult = glyphParser->ParseGlyph(*this, key);
	if (!parserResult.has_value())
		return nullptr;
	glyphs[key] = *parserResult->glyphInfo;
	delete parserResult->glyphInfo;
	mappingQueue.emplace(key, parserResult->pixels);
	return &glyphs[key];
}

void ArFont::Awake()
{
	fallbackGlyph = TryGetGlyph(u'\xFFFD', 16u, ArGlyphFlag::None);
	if (!fallbackGlyph)
		fallbackGlyph = TryGetGlyph('?', 16u, ArGlyphFlag::None);
	if (!fallbackGlyph)
		fallbackGlyph = TryGetGlyph(' ', 16u, ArGlyphFlag::None);
}

void ArFont::EndFrame(ArgonRenderManager& renderManager)
{
	while (!mappingQueue.empty())
	{
		GlyphMappingTask& task = mappingQueue.front();
		mappingQueue.pop();
		auto it = glyphs.find(task.key);
		if (it == glyphs.end())
			continue;
		ArGlyphInfo& glyphInfo = it->second;

		ArgonRenderManager::Territory territory = renderManager.AllocateTerritory((int)glyphInfo.size.x, (int)glyphInfo.size.y);
		ArTextureLand& land = renderManager.lands[territory.landIndex];

		glyphInfo.landIndex = territory.landIndex;

		for (int y = 0; y < territory.size.y; y++)
		{
			uint32_t* write_ptr = &land.pixels[territory.position.x + ((territory.position.y + y) * land.size.x)];
			for (int x = 0; x < territory.size.x; x++)
			{
				*(write_ptr + x) = task.pixels[x + y * territory.size.x];
			}
		}

		delete[] task.pixels;

		glyphInfo.uv = ArRect(territory.position * land.uvScale,
			(territory.position + territory.size) * land.uvScale);
		land.dirty = true;
	}
}

// ---------------------------------------------------------- //
// !. ArgonRenderManager implementation
// ---------------------------------------------------------- //

void ArgonRenderManager::Awake()
{
	sharedData.renderManager = this;

	BuildBaseTerritories(sharedData);

	for (auto& font : fonts)
	{
		font.Awake();
	}
}

void ArgonRenderManager::StartFrame(IArgonGlyphParser* glyphParser)
{
	while (!newFontRequests.empty())
	{
		NewFontRequest& query = newFontRequests.front();
		auto result = glyphParser->InitFontFace((uint8_t*)query.fontBinary, query.binarySize);
		newFontRequests.pop();
		if (!result.has_value())
			continue;
		fonts.emplace_back(glyphParser, result->fontId, result->name);
	}

	totalVertexCount = totalIndexCount = 0;
}

void ArgonRenderManager::EndFrame()
{
	if (fonts.empty())
		AddFontFromCompressedBase85((const uint8_t*)proggy_clean_ttf_compressed_data_base85);

	for (auto& font : fonts)
	{
		font.EndFrame(*this);
	}

	for (auto& renderList : renderLists)
	{
		totalVertexCount += renderList->vertices.size();
		totalIndexCount += renderList->indices.size();
	}
}

void ArgonRenderManager::PostPresent()
{
	for (auto& renderList : renderLists)
	{
		delete renderList;
	}
	renderLists.clear();
}

void ArgonRenderManager::OnDestroy(IArgonRenderer* renderer)
{
	renderLists.clear();

	for (auto& land : lands)
	{
		land.OnDestroy(renderer);
	}
	lands.clear();

	while (!newFontRequests.empty())
	{
		NewFontRequest& query = newFontRequests.front();
		free(query.fontBinary);
		newFontRequests.pop();
	}
}

void ArgonRenderManager::AddFontFromCompressed(void* data, size_t size)
{
	const uint32_t decompressedSize = stb_decompress_length((const uint8_t*)data);
	void* decompressedData = malloc(decompressedSize);
	stb_decompress((uint8_t*)decompressedData, (const uint8_t*)data, (uint32_t)size);
	newFontRequests.emplace(decompressedData, decompressedSize);
}

void ArgonRenderManager::AddFontFromCompressedBase85(const uint8_t* base85)
{
	size_t binarySize = (((size_t)strlen((char*)base85) + 4) / 5) * 4;
	void* binary = malloc(binarySize);
	Decode85(base85, (uint8_t*)binary);
	AddFontFromCompressed(binary, binarySize);
}

void ArgonRenderManager::AddFontFromPath(const std::filesystem::path& path)
{
	std::ifstream file = std::ifstream(path, std::ios::binary);
	if (!file.is_open())
		return;

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	void* binary = malloc(size);
	file.seekg(0, std::ios::beg);
	file.read((char*)binary, size);
	file.close();

	newFontRequests.emplace(binary, size);
}

ArImage ArgonRenderManager::AddImage(uint32_t* pixels, const ArIntVec2& size)
{
	Territory territory = AllocateTerritory(size.x, size.y);
	ArTextureLand& land = lands[territory.landIndex];
	for (int y = 0; y < size.y; y++)
	{
		uint32_t* write_ptr = &land.pixels[territory.position.x + ((territory.position.y + y) * land.size.x)];
		for (int x = 0; x < territory.size.x; x++)
			*(write_ptr + x) = pixels[x + y * size.x];
	}
	land.dirty = true;
	ArTextureID texture = land.GetTexture();
	ArRect uv = ArRect(territory.position * land.uvScale,
		(territory.position + territory.size) * land.uvScale);
	ArImage image = ArImage();
	image.texture = texture;
	image.uv = uv;
	image.scale = ArVec2(1.f / size.x, 1.f / size.y);
	image.size = territory.size;
	return image;
}

ArTextureID ArgonRenderManager::GetDefaultTexture() const
{
	if (lands.empty())
		return nullptr;

	return lands[0].GetTexture();
}

ArGlyphInfo* ArgonRenderManager::TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags) const
{
	for (auto& font : fonts)
	{
		ArGlyphInfo* glyphInfo = font.GetGlyphNoFallback(codepoint, size, flags);
		if (glyphInfo)
			return glyphInfo;
	}
	return nullptr;
}

std::vector<ArGlyphInfo*> ArgonRenderManager::TryGetGlyphs(ArStringView text, uint32_t size, ArGlyphFlag flags) const
{
	std::vector<uint32_t> codePoints = ArHelp::Utf8::DecodeToCodepoints(text);
	std::vector<ArGlyphInfo*> glyphInfos;
	glyphInfos.reserve(codePoints.size());
	for (auto& code : codePoints)
	{
		glyphInfos.push_back(TryGetGlyph(code, size, flags));
	}
	return glyphInfos;
}

ArVec2 ArgonRenderManager::CalcTextSize(ArStringView text, uint32_t size, ArGlyphFlag flags) const
{
	std::vector<ArGlyphInfo*> glyphInfos = TryGetGlyphs(text, size, flags);
	ArVec2 textSize = ArVec2(0, 0);
	for (auto& glyphInfo : glyphInfos)
	{
		if (!glyphInfo)
			continue;
		textSize.x += glyphInfo->advanceX;
		textSize.y = ArMax(textSize.y, glyphInfo->min.y + glyphInfo->size.y);
	}
	textSize.x = ArMax(textSize.x, 1.0f);
	textSize.y = ArMax(textSize.y, 1.0f);
	return textSize;
}

ArgonRenderManager::Territory ArgonRenderManager::AllocateTerritory(int width, int height)
{
	if (width <= 0 || height <= 0)
		return {};

	stbrp_rect rect = {};
	rect.w = width + TerritoryPadding * 2;
	rect.h = height + TerritoryPadding * 2;

	auto landIt = lands.begin();

	Territory territory = Territory();
	territory.size = ArIntVec2(width, height);
	do
	{
		if (landIt == lands.end())
		{
			lands.emplace_back();
			landIt = std::prev(lands.end());
		}
		auto& land = *landIt;

		if (stbrp_pack_rects(&land.rectPackerContext, &rect, 1))
		{
			territory.landIndex = (uint32_t)std::distance(lands.begin(), landIt);
			territory.position = ArIntVec2(rect.x + TerritoryPadding, rect.y + TerritoryPadding);
			break;
		}

		landIt++;
	} while (!rect.was_packed);
	territories.push_back(territory);
	return territory;
}

void ArgonRenderManager::BuildBaseTerritories(ArRenderListSharedData& sharedData)
{
	Territory antiAliasingGlyph = AllocateTerritory(64, 64);
	ArTextureLand& land = lands[antiAliasingGlyph.landIndex];
	for (int n = 0; n < 64; n++)
	{
		uint32_t y = n;
		uint32_t line_width = n;
		uint32_t pad_left = (antiAliasingGlyph.size.x - line_width) / 2;
		uint32_t pad_right = antiAliasingGlyph.size.x - (pad_left + line_width);


		uint32_t* write_ptr = &land.pixels[antiAliasingGlyph.position.x + ((antiAliasingGlyph.position.y + y) * land.size.x)];
		for (uint32_t i = 0; i < pad_left; i++)
			*(write_ptr + i) = AR_COL32(255, 255, 255, 0);

		for (uint32_t i = 0; i < line_width; i++)
			*(write_ptr + pad_left + i) = AR_COL32_WHITE;

		for (uint32_t i = 0; i < pad_right; i++)
			*(write_ptr + pad_left + line_width + i) = AR_COL32(255, 255, 255, 0);

		ArVec2 uv0 = ArVec2(antiAliasingGlyph.position.x + pad_left - 1, antiAliasingGlyph.position.y + y) * land.uvScale;
		ArVec2 uv1 = ArVec2(antiAliasingGlyph.position.x + pad_left + line_width + 1, antiAliasingGlyph.position.y + y + 1) * land.uvScale;
		float half_v = (uv0.y + uv1.y) * 0.5f;
		sharedData.uvOfBakeLines[n] = ArVec4(uv0.x, half_v, uv1.x, half_v);
	}

	Territory whitePixel = AllocateTerritory(2, 2);
	for (int y = 0; y < whitePixel.size.y; y++)
	{
		uint32_t* write_ptr = &land.pixels[whitePixel.position.x + ((whitePixel.position.y + y) * land.size.x)];
		for (int x = 0; x < whitePixel.size.x; x++)
			*(write_ptr + x) = AR_COL32_WHITE;
	}
	sharedData.uvOfWhitInTexture = ArVec2(whitePixel.position.x + 0.5f, whitePixel.position.y + 0.5f) * land.uvScale;
}
