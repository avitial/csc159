
a.out:     file format elf32-i386


Disassembly of section .init:

08048294 <_init>:
 8048294:	55                   	push   %ebp
 8048295:	89 e5                	mov    %esp,%ebp
 8048297:	53                   	push   %ebx
 8048298:	83 ec 04             	sub    $0x4,%esp
 804829b:	e8 00 00 00 00       	call   80482a0 <_init+0xc>
 80482a0:	5b                   	pop    %ebx
 80482a1:	81 c3 10 14 00 00    	add    $0x1410,%ebx
 80482a7:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 80482ad:	85 d2                	test   %edx,%edx
 80482af:	74 05                	je     80482b6 <_init+0x22>
 80482b1:	e8 1e 00 00 00       	call   80482d4 <__gmon_start__@plt>
 80482b6:	e8 e5 00 00 00       	call   80483a0 <frame_dummy>
 80482bb:	e8 00 02 00 00       	call   80484c0 <__do_global_ctors_aux>
 80482c0:	58                   	pop    %eax
 80482c1:	5b                   	pop    %ebx
 80482c2:	c9                   	leave  
 80482c3:	c3                   	ret    

Disassembly of section .plt:

080482c4 <__gmon_start__@plt-0x10>:
 80482c4:	ff 35 b4 96 04 08    	pushl  0x80496b4
 80482ca:	ff 25 b8 96 04 08    	jmp    *0x80496b8
 80482d0:	00 00                	add    %al,(%eax)
	...

080482d4 <__gmon_start__@plt>:
 80482d4:	ff 25 bc 96 04 08    	jmp    *0x80496bc
 80482da:	68 00 00 00 00       	push   $0x0
 80482df:	e9 e0 ff ff ff       	jmp    80482c4 <_init+0x30>

080482e4 <__libc_start_main@plt>:
 80482e4:	ff 25 c0 96 04 08    	jmp    *0x80496c0
 80482ea:	68 08 00 00 00       	push   $0x8
 80482ef:	e9 d0 ff ff ff       	jmp    80482c4 <_init+0x30>

080482f4 <printf@plt>:
 80482f4:	ff 25 c4 96 04 08    	jmp    *0x80496c4
 80482fa:	68 10 00 00 00       	push   $0x10
 80482ff:	e9 c0 ff ff ff       	jmp    80482c4 <_init+0x30>

Disassembly of section .text:

08048310 <_start>:
 8048310:	31 ed                	xor    %ebp,%ebp
 8048312:	5e                   	pop    %esi
 8048313:	89 e1                	mov    %esp,%ecx
 8048315:	83 e4 f0             	and    $0xfffffff0,%esp
 8048318:	50                   	push   %eax
 8048319:	54                   	push   %esp
 804831a:	52                   	push   %edx
 804831b:	68 50 84 04 08       	push   $0x8048450
 8048320:	68 60 84 04 08       	push   $0x8048460
 8048325:	51                   	push   %ecx
 8048326:	56                   	push   %esi
 8048327:	68 fe 83 04 08       	push   $0x80483fe
 804832c:	e8 b3 ff ff ff       	call   80482e4 <__libc_start_main@plt>
 8048331:	f4                   	hlt    
 8048332:	90                   	nop
 8048333:	90                   	nop
 8048334:	90                   	nop
 8048335:	90                   	nop
 8048336:	90                   	nop
 8048337:	90                   	nop
 8048338:	90                   	nop
 8048339:	90                   	nop
 804833a:	90                   	nop
 804833b:	90                   	nop
 804833c:	90                   	nop
 804833d:	90                   	nop
 804833e:	90                   	nop
 804833f:	90                   	nop

08048340 <__do_global_dtors_aux>:
 8048340:	55                   	push   %ebp
 8048341:	89 e5                	mov    %esp,%ebp
 8048343:	53                   	push   %ebx
 8048344:	8d 64 24 fc          	lea    -0x4(%esp),%esp
 8048348:	80 3d cc 96 04 08 00 	cmpb   $0x0,0x80496cc
 804834f:	75 3e                	jne    804838f <__do_global_dtors_aux+0x4f>
 8048351:	bb dc 95 04 08       	mov    $0x80495dc,%ebx
 8048356:	a1 d0 96 04 08       	mov    0x80496d0,%eax
 804835b:	81 eb d8 95 04 08    	sub    $0x80495d8,%ebx
 8048361:	c1 fb 02             	sar    $0x2,%ebx
 8048364:	83 eb 01             	sub    $0x1,%ebx
 8048367:	39 d8                	cmp    %ebx,%eax
 8048369:	73 1d                	jae    8048388 <__do_global_dtors_aux+0x48>
 804836b:	90                   	nop
 804836c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8048370:	83 c0 01             	add    $0x1,%eax
 8048373:	a3 d0 96 04 08       	mov    %eax,0x80496d0
 8048378:	ff 14 85 d8 95 04 08 	call   *0x80495d8(,%eax,4)
 804837f:	a1 d0 96 04 08       	mov    0x80496d0,%eax
 8048384:	39 d8                	cmp    %ebx,%eax
 8048386:	72 e8                	jb     8048370 <__do_global_dtors_aux+0x30>
 8048388:	c6 05 cc 96 04 08 01 	movb   $0x1,0x80496cc
 804838f:	8d 64 24 04          	lea    0x4(%esp),%esp
 8048393:	5b                   	pop    %ebx
 8048394:	5d                   	pop    %ebp
 8048395:	c3                   	ret    
 8048396:	8d 76 00             	lea    0x0(%esi),%esi
 8048399:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

080483a0 <frame_dummy>:
 80483a0:	55                   	push   %ebp
 80483a1:	89 e5                	mov    %esp,%ebp
 80483a3:	8d 64 24 e8          	lea    -0x18(%esp),%esp
 80483a7:	a1 e0 95 04 08       	mov    0x80495e0,%eax
 80483ac:	85 c0                	test   %eax,%eax
 80483ae:	74 12                	je     80483c2 <frame_dummy+0x22>
 80483b0:	b8 00 00 00 00       	mov    $0x0,%eax
 80483b5:	85 c0                	test   %eax,%eax
 80483b7:	74 09                	je     80483c2 <frame_dummy+0x22>
 80483b9:	c7 04 24 e0 95 04 08 	movl   $0x80495e0,(%esp)
 80483c0:	ff d0                	call   *%eax
 80483c2:	c9                   	leave  
 80483c3:	c3                   	ret    

080483c4 <DispMsg>:
 80483c4:	55                   	push   %ebp
 80483c5:	89 e5                	mov    %esp,%ebp
 80483c7:	83 ec 28             	sub    $0x28,%esp
 80483ca:	c7 45 f4 05 00 00 00 	movl   $0x5,-0xc(%ebp)
 80483d1:	b8 14 85 04 08       	mov    $0x8048514,%eax
 80483d6:	8b 55 08             	mov    0x8(%ebp),%edx
 80483d9:	89 54 24 04          	mov    %edx,0x4(%esp)
 80483dd:	89 04 24             	mov    %eax,(%esp)
 80483e0:	e8 0f ff ff ff       	call   80482f4 <printf@plt>
 80483e5:	b8 1d 85 04 08       	mov    $0x804851d,%eax
 80483ea:	8b 55 0c             	mov    0xc(%ebp),%edx
 80483ed:	89 54 24 04          	mov    %edx,0x4(%esp)
 80483f1:	89 04 24             	mov    %eax,(%esp)
 80483f4:	e8 fb fe ff ff       	call   80482f4 <printf@plt>
 80483f9:	8b 45 f4             	mov    -0xc(%ebp),%eax
 80483fc:	c9                   	leave  
 80483fd:	c3                   	ret    

080483fe <main>:
 80483fe:	55                   	push   %ebp
 80483ff:	89 e5                	mov    %esp,%ebp
 8048401:	83 e4 f0             	and    $0xfffffff0,%esp
 8048404:	83 ec 20             	sub    $0x20,%esp
 8048407:	c7 44 24 14 01 00 00 	movl   $0x1,0x14(%esp)
 804840e:	00 
 804840f:	c7 44 24 18 02 00 00 	movl   $0x2,0x18(%esp)
 8048416:	00 
 8048417:	8b 44 24 18          	mov    0x18(%esp),%eax
 804841b:	89 44 24 04          	mov    %eax,0x4(%esp)
 804841f:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048423:	89 04 24             	mov    %eax,(%esp)
 8048426:	e8 99 ff ff ff       	call   80483c4 <DispMsg>
 804842b:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 804842f:	b8 26 85 04 08       	mov    $0x8048526,%eax
 8048434:	8b 54 24 1c          	mov    0x1c(%esp),%edx
 8048438:	89 54 24 04          	mov    %edx,0x4(%esp)
 804843c:	89 04 24             	mov    %eax,(%esp)
 804843f:	e8 b0 fe ff ff       	call   80482f4 <printf@plt>
 8048444:	c9                   	leave  
 8048445:	c3                   	ret    
 8048446:	90                   	nop
 8048447:	90                   	nop
 8048448:	90                   	nop
 8048449:	90                   	nop
 804844a:	90                   	nop
 804844b:	90                   	nop
 804844c:	90                   	nop
 804844d:	90                   	nop
 804844e:	90                   	nop
 804844f:	90                   	nop

08048450 <__libc_csu_fini>:
 8048450:	55                   	push   %ebp
 8048451:	89 e5                	mov    %esp,%ebp
 8048453:	5d                   	pop    %ebp
 8048454:	c3                   	ret    
 8048455:	66 66 2e 0f 1f 84 00 	data32 nopw %cs:0x0(%eax,%eax,1)
 804845c:	00 00 00 00 

08048460 <__libc_csu_init>:
 8048460:	55                   	push   %ebp
 8048461:	89 e5                	mov    %esp,%ebp
 8048463:	57                   	push   %edi
 8048464:	56                   	push   %esi
 8048465:	53                   	push   %ebx
 8048466:	e8 4f 00 00 00       	call   80484ba <__i686.get_pc_thunk.bx>
 804846b:	81 c3 45 12 00 00    	add    $0x1245,%ebx
 8048471:	83 ec 1c             	sub    $0x1c,%esp
 8048474:	e8 1b fe ff ff       	call   8048294 <_init>
 8048479:	8d bb 20 ff ff ff    	lea    -0xe0(%ebx),%edi
 804847f:	8d 83 20 ff ff ff    	lea    -0xe0(%ebx),%eax
 8048485:	29 c7                	sub    %eax,%edi
 8048487:	c1 ff 02             	sar    $0x2,%edi
 804848a:	85 ff                	test   %edi,%edi
 804848c:	74 24                	je     80484b2 <__libc_csu_init+0x52>
 804848e:	31 f6                	xor    %esi,%esi
 8048490:	8b 45 10             	mov    0x10(%ebp),%eax
 8048493:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048497:	8b 45 0c             	mov    0xc(%ebp),%eax
 804849a:	89 44 24 04          	mov    %eax,0x4(%esp)
 804849e:	8b 45 08             	mov    0x8(%ebp),%eax
 80484a1:	89 04 24             	mov    %eax,(%esp)
 80484a4:	ff 94 b3 20 ff ff ff 	call   *-0xe0(%ebx,%esi,4)
 80484ab:	83 c6 01             	add    $0x1,%esi
 80484ae:	39 fe                	cmp    %edi,%esi
 80484b0:	72 de                	jb     8048490 <__libc_csu_init+0x30>
 80484b2:	83 c4 1c             	add    $0x1c,%esp
 80484b5:	5b                   	pop    %ebx
 80484b6:	5e                   	pop    %esi
 80484b7:	5f                   	pop    %edi
 80484b8:	5d                   	pop    %ebp
 80484b9:	c3                   	ret    

080484ba <__i686.get_pc_thunk.bx>:
 80484ba:	8b 1c 24             	mov    (%esp),%ebx
 80484bd:	c3                   	ret    
 80484be:	90                   	nop
 80484bf:	90                   	nop

080484c0 <__do_global_ctors_aux>:
 80484c0:	55                   	push   %ebp
 80484c1:	89 e5                	mov    %esp,%ebp
 80484c3:	53                   	push   %ebx
 80484c4:	8d 64 24 fc          	lea    -0x4(%esp),%esp
 80484c8:	a1 d0 95 04 08       	mov    0x80495d0,%eax
 80484cd:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484d0:	74 12                	je     80484e4 <__do_global_ctors_aux+0x24>
 80484d2:	bb d0 95 04 08       	mov    $0x80495d0,%ebx
 80484d7:	90                   	nop
 80484d8:	8d 5b fc             	lea    -0x4(%ebx),%ebx
 80484db:	ff d0                	call   *%eax
 80484dd:	8b 03                	mov    (%ebx),%eax
 80484df:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484e2:	75 f4                	jne    80484d8 <__do_global_ctors_aux+0x18>
 80484e4:	8d 64 24 04          	lea    0x4(%esp),%esp
 80484e8:	5b                   	pop    %ebx
 80484e9:	5d                   	pop    %ebp
 80484ea:	c3                   	ret    
 80484eb:	90                   	nop

Disassembly of section .fini:

080484ec <_fini>:
 80484ec:	55                   	push   %ebp
 80484ed:	89 e5                	mov    %esp,%ebp
 80484ef:	53                   	push   %ebx
 80484f0:	83 ec 04             	sub    $0x4,%esp
 80484f3:	e8 00 00 00 00       	call   80484f8 <_fini+0xc>
 80484f8:	5b                   	pop    %ebx
 80484f9:	81 c3 b8 11 00 00    	add    $0x11b8,%ebx
 80484ff:	e8 3c fe ff ff       	call   8048340 <__do_global_dtors_aux>
 8048504:	59                   	pop    %ecx
 8048505:	5b                   	pop    %ebx
 8048506:	c9                   	leave  
 8048507:	c3                   	ret    
