// mk.h
// Created by Fred Nora.

#ifndef __MK_H
#define __MK_H    1

/*
 * Segments @todo: Include this constants.
 * it's Intel's specific stuff.
 *
#define GDT_KCODE   0x08
#define GDT_KDATA   0x10
#define GDT_UCODE   (0x18 | 3)
#define GDT_UDATA   (0x20 | 3)
*/

//
//  ## JOB CONTROL SUPPORT ##
//

#define PROCESS    10
#define THREAD     20
#define BACKGROUND  100 
#define FOREGROUND  200


// int microkernelStatus; 
 

//
// == prototypes =================================================
//

int jobcontrol ( int type, int id, int plane );
int jobcheck ( int type, int id );

void microkernelTestLimit (void);
 
// Inicializa o microkernel do kernel.
int init_microkernel (void);


#endif    


//
// End.
//

