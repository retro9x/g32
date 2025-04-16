/*
 * File: wm.c 
 *
 *  wm - Kernel Window Manager.
 * 
 * Descri��o:
 *     Arquivo principal do Window Manager.
 *     Window Manager.
 *     MB - M�dulos inclu�dos no Kernel Base.
 *     Rotinas envolvendo a feitura de janelas.     
 *     Parte da interface gr�fica oferecida em Kernel Mode.
 *     A interface gr�fica oferecida pelo Kernel Base deve ser leve e veloz.
 *
 * Obs: Muitas funcionalidades devem ser iplementadas em user mode e n�o aqui.
 *      aqui devem ficar as fun��es mais primitivas. e outras pra ganhar desempenho
 *      mas n�o tudo. 
 *
 * #todo: 
 * Criar uma fun��o que copie o conte�do de um buffer dedicado de janela para
 * o LFB,(FrontBuffer).
 *       
 *
 * History:
 *    2013 - Created by Fred Nora.
 */


// #obs:
// Tem coisas aqui que pertencem ao window server e
// coisas que pertencem ao window manager ou s�o de suporte ao 
// window manager. 


#include <kernel.h>
 

// 
// Argumentos passados pelo Boot Loader em head.s. 
//

extern unsigned long kArg1;    //Video Mode.	
extern unsigned long kArg2;    //LFB - Linear Frame Buffer.
extern unsigned long kArg3;    //??.
extern unsigned long kArg4;    //??.


/*
 #importante:
 Isso � uma rotina de movimento de janela.
 vamos construir uma fun��o pra isso.
 
 					save_window ( (struct window_d *) windowList[window_with_focus] );
					replace_window ( (struct window_d *) windowList[window_with_focus], 20, 20 );
					show_saved_window ( (struct window_d *) windowList[window_with_focus] );
					show_window_rect ( (struct window_d *) windowList[window_with_focus] );
 */



//salva o ret�ngulo de uma janela no buffer de salvamento.
//isso ser� usado para mover a janela.
//wm.
int save_window (struct window_d *window){


    if ( (void *) window == NULL ){
        printf ("save_window: struct\n");
        return 1;

    }else{
        if ( window->used != 1 || window->magic != 1234 ){
            printf ("save_window: validation\n");
            return 1;
         }

        //#debug
        //Ok. est� pegando os valores certos.
        //printf ("l=%d t=%d w=%d h=%d \n", window->left, window->top, window->width, window->height );
        //refresh_screen();
        //while(1){}


        save_rect ( window->left, window->top, 
            window->width, window->height );
    };
}


//mostrar uma janela que tem seu ret�ngulo salvo no buffer de salvamento.
//#obs: antes de chamarmos essa fun��o podemos chamar a fun��o replace_window
//para mudar a janela de lugar.

//wm.

int show_saved_window (struct window_d *window){

    if( (void *) window == NULL ){
		return 1;

	}else{
	
        if ( window->used != 1 || window->magic != 1234 )
        {
			return 1;
		}

		show_saved_rect ( window->left, window->top, 
		    window->width, window->height );
        
        //... 		
	};		
}


/*
 ***********************************************
 * show_window_rect:
 *     Mostra o ret�ngulo de uma janela que est� no backbuffer.
 *     Tem uma janela no backbuffer e desejamos enviar ela para o frontbuffer.
 *     A rotina de refresh rectangle tem que ter o vsync
 *     #todo: criar um define chamado refresh_window.
 */

int show_window_rect (struct window_d *window)
{

    if ( (void *) window == NULL )
    {
        printf ("show_window_rect: window\n");
        return (int) -1;

    } else {

        if ( window->used == 1 || window->magic == 1234 )
        {
			
			//#shadow 
			// ?? E se a janela tiver uma sombra, 
			// ent�o precisamos mostrar a sombra tamb�m. 
			
			//#bugbug
			//Extranhamente essa checagem atraza a pintura da janela.
			//Ou talvez o novo tamanho favore�a o refresh rectangle,
			//ja que tem rotinas diferentes para larguras diferentes
			
			//if ( window->shadowUsed == 1 )
			//{

			    //window->width = window->width +4;
				//window->height = window->height +4;
			    
				//refresh_rectangle ( window->left, window->top, 
				//    window->width +2, window->height +2 ); 
				//return (int) 0;
			//}

            // #hackhack
            // Inclui esse 8 ate descobrir porque nao esta 
            // dando refresh da janela inteira.

            refresh_rectangle ( 
                window->left, window->top, 
                window->width, window->height); 

            return 0;
        }
    };

    // fail.
    return (int) -1;
}


int windowKillTimer ( struct window_d *window, int id ){
	
    int Status = 0;
	
	if( (void *) window == NULL )
	{
		Status = 1;
		goto fail;
		
	}else{
		
		if ( window->used != 1 || window->magic != 1234 )
		{
			Status = 1;
		    goto fail;
		}
    
	    //...
	};

fail:
//done:

    return (int) Status;
}


/*
 *windowSetTimer:
 * 
 */
// A janela receber� uma mensagem MSG_TIMER 
// sempre que o contador chegar a '0'.
// +Essa rotina deve utilizar o driver de timer 
// para conseguir a contagem que precisa.
// #teste, come�aremos contagem simples para teste.

// IN:
// janela, id do timer, tempo (medida indefinida ainda)

int
windowSetTimer ( 
    struct window_d *window, 
    int id,  
    int time )  
{

    int Status = 0;


	if( (void *) window == NULL ){
		Status = 1;
		goto fail;

	}else{
		if ( window->used != 1 || window->magic != 1234 )
		{
			Status = 1;
		    goto fail;
		}
		
		//Ok.
		// Aqui devemos registra na estrutura 
		// da janela o id do timer e o tempo 
        //desejado. 		
		
		//obs: temos um limite para ids.
		//window->timer[id].id = id;
		//window->timer[id].time = time;
		
		//...
	};
		
fail:
done:	

    return (int) Status;
}


/*
  test - queue support *******************************
 */
 
 
 /*
 
#define MAX_ITEMS    4*4
typedef struct circularQueue_s
{
    int     first;
    int     last;
    int     validItems;
    
	//int     data[MAX_ITEMS];
	unsigned long data[MAX_ITEMS];
	
} circularQueue_t;


    unsigned long readValue;
    circularQueue_t   myQueue;

void initializeQueue(circularQueue_t *theQueue);
 
int isEmpty(circularQueue_t *theQueue);
 
int putItem(circularQueue_t *theQueue, unsigned long theItemValue);
 
int getItem(circularQueue_t *theQueue, unsigned long *theItemValue);
 
//void printQueue(circularQueue_t *theQueue);

*/

/*
void initializeQueue(circularQueue_t *theQueue)
{
    int i;
    theQueue->validItems  =  0;
    theQueue->first       =  0;
    theQueue->last        =  0;
    for(i=0; i<MAX_ITEMS; i++)
    {
        theQueue->data[i] = (unsigned long) 0;
    }        
    return;
}
*/

/*
int isEmpty(circularQueue_t *theQueue)
{
    if(theQueue->validItems==0)
        return(1);
    else
        return(0);
}
*/

/*
int putItem(circularQueue_t *theQueue, unsigned long theItemValue)
{
    if(theQueue->validItems>=MAX_ITEMS)
    {
        printf("The queue is full\n");
        printf("You cannot add items\n");
        return(-1);
    }
    else
    {
        theQueue->validItems++;
        theQueue->data[theQueue->last] = (unsigned long) theItemValue;
        theQueue->last = (theQueue->last+1)%MAX_ITEMS;
    }
}
*/

/*
int getItem(circularQueue_t *theQueue, unsigned long *theItemValue)
{
    if(isEmpty(theQueue))
    {
        printf("isempty\n");
        return(-1);
    }
    else
    {
        *theItemValue =  theQueue->data[theQueue->first];
        theQueue->first = (theQueue->first+1)%MAX_ITEMS;
        theQueue->validItems--;
        return(0);
    }
}
*/

/*
void printQueue(circularQueue_t *theQueue)
{
    int aux, aux1;
    aux  = theQueue->first;
    aux1 = theQueue->validItems;
    while(aux1>0)
    {
        printf("Element #%d = %d\n", aux, theQueue->data[aux]);
        aux=(aux+1)%MAX_ITEMS;
        aux1--;
    }
    return;
}
 
*/

/*
 **************************************
*/ 


//retorna o id de uma janela.
//wm.

int windowGetWindowID ( struct window_d *window ){

	if ( (void *) window != NULL )
	{
		if ( window->used == 1 && window->magic == 1234 )
		{	
		    return (int) window->id; 	
		}
	}

    return (int) -1;
}


// Pegar o id da janela main.
// Para os aplicativos lidarem com a �rea de trabalho. 

// wm.

int windowGetMainWindowDescriptor (void){
	
	if ( (void *) gui == NULL ){
		goto fail;
	
	}else{
		
		if ( (void *) gui->main == NULL )
		{
		    goto fail;
		}
		
		return (int) gui->main->id;
	};
	
fail:	
	
	return (int) -1;
}


/*
 * @todo: Talvez aque devamos apenas atualizar a flag de view da janela.
int windowShowWindow(struct window_d *window, int msg);
int windowShowWindow(struct window_d *window, int msg)
{
	int Status;
	
	if( (void*) window == NULL ){
		return (int) 1;
	};
	
	
	switch(cmd)
	{
		case MSG_HIDE:
		case MSG_MAXIMIZE:
		case MSG_RESTORE:
		case MSG_SHOWDEFAULT:
		default:
	}
	
	//@todo: Copiar o conteudo do buffer dedicado para o frontbuffer.
	
	//na estrutura de janela:
	//a vari�vel dedicatedbuffer precisa estar correta.
	//a vari�vel frontbuffer precisa estar correta.
	
	return (int) Status;
}
*/



/*
 * windowSetParent:
 * Atribui uma nova janela mae � uma dada janela filha.
 */
/*
void *windowSetParent(struct window_t *clild, struct window_d *new_parent);
void *windowSetParent(struct window_t *clild, struct window_d *new_parent)
{
	//@implementar.
	//Obs: Se o par�metro new_parent for NULL ent�o a janela principal (main ou desktop) 
	//ser� a nova janela mae.
	return NULL;
}
*/

/*
 * windowIsWindow:
 *     Testa atrav�s do handle passado, se � uma janela ou n�o.
 *     0 = Yes; 1 = Not; 
 */
/* 
int windowIsWindow(struct window_d *window);
int windowIsWindow(struct window_d *window)
{
	//@todo:Implementar.
	return 0;
};
*/

/*
void* windowGetWindowWithFocus();
void* windowGetWindowWithFocus()
{
	return (void*) WindowWithFocus;
}
*/

//configuramos qual janela ficar� por cima de todas as outras.
/*
void windowSetForegroundWindow(struct window_d *window);
void windowSetForegroundWindow(struct window_d *window)
{
	//set focus 
	//coloca no topo da lista
	return;
}
*/


//get parent window
void *GetParentWindow (struct window_d * hwnd){

    if ( (void *) hwnd == NULL ){
        //message
        return NULL;
    }

    return (void *) hwnd->parent;
}


//get the desktop of a window.
void *GetWindowDesktop (struct window_d *hwnd){

    if ( (void *) hwnd == NULL ){
        //message
        return NULL;
    }

    return (void *) hwnd->desktop;
}


/*
 * =====================================================
 * windowInitializeColorSchemeSupport:
 *     Isso configura os esquemas de cores utilizados 
 * pelo sistema.
 *     Essa rotina � chamada apenas uma vez na inicializa��o
 * do kernel.
 *     O esquema de cores a ser utilizado deve estar 
 * salvo no perfil do usu�rio que fez o logon.
 * Os dois esquemas padr�o chamam-se: 'humility' e 'pride'.
 * + O esquema 'humility' s�o cores com tema cinza, 
 * lembrando interfaces antigas.
 * + O esquema 'pride' s�o cores modernas 
 *   ( Aquele verde e preto e cinza, das primeiras vers�es, 
 * com imagens publicadas. )
 * @todo: � preciso criar rotinas que selecionem entre os 
 * modo criados e habilitados.
 * � preciso criar rotinas que permitam que aplicativos 
 * em user mode criem esquemas de cores e habilite eles.
 */

int windowInitializeColorSchemeSupport (void){

    struct color_scheme_d  *humility;
    struct color_scheme_d  *pride;

    int Type = ColorSchemeHumility;


//
// HUMILITY
//

    //Criando o esquema de cores humility. (cinza)
    humility = (void *) kmalloc ( sizeof(struct color_scheme_d) );

    if( (void *) humility == NULL ){
        panic ("windowSetUpColorScheme: humility");
    }else{
        humility->objectType  = ObjectTypeColorScheme;
        humility->objectClass = ObjectClassGuiObjects;
        humility->used  = TRUE;
        humility->magic = 1234;
        humility->name  = "Humility";

        // Colors
        // Definidas em ws.h
        humility->elements[csiNull] = 0;                             //0
        humility->elements[csiDesktop] = HUMILITY_COLOR_BACKGROUND;  //1
        humility->elements[csiWindow] = HUMILITY_COLOR_WINDOW;       //2
        humility->elements[csiWindowBackground] = HUMILITY_COLOR_WINDOW_BACKGROUND;       //3
        humility->elements[csiActiveWindowBorder] = HUMILITY_COLOR_ACTIVE_WINDOW_BORDER;  //4
        humility->elements[csiInactiveWindowBorder] = HUMILITY_COLOR_INACTIVE_WINDOW_BORDER;  //5
        humility->elements[csiActiveWindowTitleBar] = HUMILITY_COLOR_ACTIVE_WINDOW_TITLEBAR;  //6
        humility->elements[csiInactiveWindowTitleBar] = HUMILITY_COLOR_INACTIVE_WINDOW_TITLEBAR;  //7
        humility->elements[csiMenuBar] = HUMILITY_COLOR_MENUBAR;                //8
        humility->elements[csiScrollBar] = HUMILITY_COLOR_SCROLLBAR;            //9  
        humility->elements[csiStatusBar] = HUMILITY_COLOR_STATUSBAR;            //10
        humility->elements[csiMessageBox] = HUMILITY_COLOR_MESSAGEBOX;		    //11
        humility->elements[csiSystemFontColor] = HUMILITY_COLOR_SYSTEMFONT;		//12
        humility->elements[csiTerminalFontColor] = HUMILITY_COLOR_TERMINALFONT;	//13
        // ...

		//Sanvando na estrutura padr�o para o esquema humility.
		HumilityColorScheme = (void*) humility;
    };


//
// PRIDE 
//

    //Criando o esquema de cores PRIDE. (colorido)
    pride = (void *) kmalloc ( sizeof(struct color_scheme_d) );
    
    if ( (void *) pride == NULL ){
        panic ("windowSetUpColorScheme: pride");
    }else{
        pride->objectType  = ObjectTypeColorScheme;
        pride->objectClass = ObjectClassGuiObjects;
        pride->used  = TRUE;
        pride->magic = 1234;
        pride->name  = "Pride";

		//Colors
		//Definidas em ws.h
        pride->elements[csiNull] = 0;
        pride->elements[csiDesktop] = PRIDE_COLOR_BACKGROUND;  
        pride->elements[csiWindow] = PRIDE_COLOR_WINDOW;
        pride->elements[csiWindowBackground] = PRIDE_COLOR_WINDOW_BACKGROUND;
        pride->elements[csiActiveWindowBorder] = PRIDE_COLOR_ACTIVE_WINDOW_BORDER; 
        pride->elements[csiInactiveWindowBorder] = PRIDE_COLOR_INACTIVE_WINDOW_BORDER; 
        pride->elements[csiActiveWindowTitleBar] = PRIDE_COLOR_ACTIVE_WINDOW_TITLEBAR; 
        pride->elements[csiInactiveWindowTitleBar] = PRIDE_COLOR_INACTIVE_WINDOW_TITLEBAR;
        pride->elements[csiMenuBar] = PRIDE_COLOR_MENUBAR;
        pride->elements[csiScrollBar] = PRIDE_COLOR_SCROLLBAR;  		
        pride->elements[csiStatusBar] = PRIDE_COLOR_STATUSBAR;    
        pride->elements[csiMessageBox] = PRIDE_COLOR_MESSAGEBOX;
        pride->elements[csiSystemFontColor] = PRIDE_COLOR_SYSTEMFONT;    //12
        pride->elements[csiTerminalFontColor] = PRIDE_COLOR_TERMINALFONT;  //13
        // ...

		//Sanvando na estrutura padr�o para o esquema pride.
		PrideColorScheme = (void *) pride;
    };


	// Configurando qual ser� o esquema padr�o.
	// @todo; Criar uma fun��o que selecione qual dois esquemas ser�o usados
	//        apenas selecionando o ponteiro da estrutura.  

    switch (Type){

    case ColorSchemeNull: 
        CurrentColorScheme = (void *) humility;
        break;

    case ColorSchemeHumility:
        CurrentColorScheme = (void *) humility;
        break;

    case ColorSchemePride:
        CurrentColorScheme = (void *) pride; 
        break;

    default:
        CurrentColorScheme = (void *) humility;
        break;
    };

    return 0;
}


//seleciona o tipo ...isso � um servi�o.
int windowSelectColorScheme (int type){
	
	//#debug
	//printf("windowSelectColorScheme: type={%d} \n", type);
	
    switch (type){

    case ColorSchemeHumility:
        goto do_humility;
        break;

    case ColorSchemePride:
        goto do_pride;
        break;

    default:
        printf ("windowSelectColorScheme: Type not defined\n");
        goto fail;
        break;
    };


do_humility:

    if ( (void *) HumilityColorScheme == NULL )
    {
		printf("HumilityColorScheme fail\n");
        goto fail;  
           	    	
	}else{
		
	    if ( HumilityColorScheme->used != 1 || 
		     HumilityColorScheme->magic != 1234 )
		{
			printf("HumilityColorScheme sig fail\n");
			goto fail;
		}
		
		printf("Humility selected\n");
	    CurrentColorScheme = HumilityColorScheme;	
	    goto done;
	};		
	
	
do_pride:	

    if ( (void *) PrideColorScheme == NULL )
    {
		printf("PrideColorScheme fail\n");
        goto fail; 
            	    	
	}else{
	    if(	PrideColorScheme->used != 1 || 
		    PrideColorScheme->magic != 1234 )
		{
			printf("PrideColorScheme sig fail\n");
			goto fail;
		}
		
	    printf ("Pride selected\n"); 
		CurrentColorScheme = PrideColorScheme;	
	    goto done;
	};		

done:
    return 0;

fail:
    printf ("fail\n");
    return 1;
}


/*
 * windowShowWWFMessageBuffers:
 *     Mostra o buffer de mensagens da janela com foco de entrada.
 *     #Rotinas de teste. @todo: Enviar para a pasta /test.
 *     Esse rotina funcionou e se mostrou bem �til.
 */

// #debug stuff.

void windowShowWWFMessageBuffers (void){

	struct window_d *wFocus;
	int i=0;


    wFocus = (void *) windowList[window_with_focus];

	if ( (void *) wFocus == NULL )
		return;
	
	//Obs: Tipo.
	
	for ( i=0; i<32; i++ ){
	    printf ("%d ", wFocus->msgList[i]);
    };
}


/*
 ***********************************************************
 * windowSendMessage:
 *     Uma mensagem ser� enviada para uma janela.
 *
 *    Uma mensagem deve ser enviada para a estrutura da janela com o 
 * foco de entrada.
 *    A classe system.devices.unblocked. pois essa rotina � um m�todo dela.
 *
 * Sobre a fila de mensagem da janela com o foco de entrada:
 * ========================================================
 *  *IMPORTANTE: 
 *      O que importa � qual janela est� com o foco de entrada.
 *      Quando sabemos qual janela est� com o foco de entrada, ent�o
 * sabemos qual � a janela que � a �rea de cliente, sabemos e provavelmente
 * a sua janela m�e � a janela ativa, se ela mesma n�o for a janela ativa.
 *      Uma mensagem pode ser enviada para a janela com o foco de entrada,
 * e a mensagem afetar� apenas a janela com o foco de entrada.
 *      Se a janela que receber a mensagem for a janela ativa, ela n�o
 * ter� janela m�e, nessa caso uma mensagem para fechar uma janela ativa
 * implica em fechar o processo ao qual ela est� vinculada.
 *      (N�o importa qual processo foi interrompido pela interrup��o de 
 * teclado. A mensagem deve ir para a fila de mensagens da janela com o foco
 * de entrada.)
 * @todo: Criar uma rotina semelhante, mas exclusivamente para a janela com
 * foco de entrada.
 * Ex: void windowSendMessageWWW(unsigned long arg1, unsigned long arg2, 
 * unsigned long arg3, unsigned long arg4)
 */	

// window server stuff. ipc.

void 
windowSendMessage ( 
    unsigned long arg1, 
    unsigned long arg2, 
    unsigned long arg3, 
    unsigned long arg4 )
{
 
    struct window_d *wFocus;


	// lembrando: 
	// O arg1 por enquanto ser� ignorado ;;; ele deveria 
	//conter a janela que devemos enviar a mensagem...
	// pegaremos por conta pr�pria o penteiro pra janela com o foco de entrada.
	
    // N�o estamos falando de fila de mensagens na estrutura da janela,
	// portando se uma mensagem for enviada para uma estrutura de janela
	// ela ir� sobrepor a mensagem antiga caso a mensagem antiga ainda n�o 
	// tenha sido consumida.

	// @todo: 
	// Na verdade, aqui tem que mandar a mensagem para a janela com o foco 
	// de entrada, mesmo que seja uma janela filha.

	//lenbrando que esse � um endere�o f�sico, o m�nimo que uma rotina em user mode deve fazer 
    //com essa informa��o � converte-la em endere�o l�gico.
    //Ou devemos enviar um endere�o l�gico ??	
	
	wFocus = (void *) windowList[window_with_focus];
	
	// Aqui estamos apenas enviando para a janela com foco de entrada.
	// N�o fazer nada caso n�o haja uma janela com o foco de entrada, 
	// pois quem decide que janela tem o foco de entrada � o usu�rio.


	if( (void *) wFocus == NULL ){
		printf("windowSendMessage: wFocus fail\n");
		refresh_screen();
	    return;
		
	}else{
		
		if ( wFocus->used == 1 && wFocus->magic == 1234 )
		{
			wFocus->msg_window = (struct window_d *) arg1;
			wFocus->msg = (int) arg2;      
			wFocus->long1 = (unsigned long) arg3;
			wFocus->long2 = (unsigned long) arg4;
			wFocus->newmessageFlag = 1;
			
			// #bugbug: 
			// Acho que isso n�o � usado.
			// isso � um teste.
			// Para facilitar vamos colocar a mensagem num lugar mais acessivel.
			
			gNextKeyboardMessage = (int) 0;
			
		}else{
			//fail.
		};
	};
}





// ++
// #todo
// Deletar essas 4 fun��es que pegavam a mensagem em partes.

void *windowGetHandleWindow (struct window_d *window)
{
    debug_print ("windowGetHandleWindow: deprecated\n");
    return NULL;
}

void *windowGetMessage (struct window_d *window)
{
    debug_print ("windowGetMessage: deprecated\n");
    return NULL;
}

void *windowGetLong1(struct window_d *window)
{
    debug_print ("windowGetLong1: deprecated\n");
    return NULL;
}

void *windowGetLong2 (struct window_d *window)
{
    debug_print ("windowGetLong2: deprecated\n");
    return NULL;
}
//--






/*
 **********************************************************
 * window_getch:
 *
 * Esse � o servi�o 137.
 * Isso � usado pela biblioteca stdio em user mode
 * na fun��o getchar()
 * #bugbug: N�o est� pegando todos os caracteres digitados.
 *
 * um aplicativo que roda no shell pode estar chamando isso.
 *
 */
 
// window server. low level.
//pode ser que esse aplicativo n�o tenha janela,
//mas esteja rodando na janela do shell.
	 
int window_getch (void)
{
    debug_print("window_getch: Deprecated!\n");
    return (int) -1; 
}



/*
 * windowCreateDedicatedBuffer: 
 *     Cria um buffer dedicado de acordo com as dimens�es da janela.
 */

// #bugbug
// Essas rotinas de buffer dedicado precisam serem revistas. 

// Buffer size.
// Alocando mem�ria no Heap do kernel.
// #todo: 
// Usar vari�vel para quantidade de bytes por pixel. 
 
int windowCreateDedicatedBuffer (struct window_d *window){

    size_t bufferSize = 0;



	if ((void *) window == NULL ){
		//message 
	    return (int) 1;  
	}
	
	bufferSize = (size_t) (window->width * 3) + (window->height * 3);
	
	if (bufferSize == 0)
	{
	    return (int) 1;    
	
	}else{
		
	    window->DedicatedBuffer = (void *) kmalloc (bufferSize);	
	    
	    if( (void *) window->DedicatedBuffer == NULL )
	    {
		    return (int) 1;    
		}
		
		return 0;
	};
	
//fail:	
	return 1;
}


/*
 * windowLock:
 *     Bloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */

// wm.

void windowLock (struct window_d *window){
	
    if ( (void *) window == NULL ){
	    return;

    }else{
        window->locked = (int) WINDOW_LOCKED;
	};
}


/*
 * windowUnlock:
 *     Desbloqueia uma janela.
 *     @todo: Quem pode realizar essa opera��o??
 */
 
// wm.
 
void windowUnlock (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
	    return;
    
    }else{
		
    	window->locked = (int) WINDOW_UNLOCKED;		
	};
}


/*
 * set_current_window:
 *     @todo: Mudar para windowSetCurrentWindow
 */

// wm.
 
void set_current_window (struct window_d *window){
	
	if ( (void *) window == NULL )
	{
		//message 
	    return; 
	} 

    current_window = (int) window->id;

    //struct.
    CurrentWindow = (void *) window;
}


/*
 * get_current_window: 
 *     Get current window pointer.
 */

// #bugbug
// Isso � o handle. Mas normalmente manipulamos o descritor.
// wm.

void *get_current_window (void)
{
    return (void *) CurrentWindow;
}


/*
int get_current_window_id();
int get_current_window_id()
{
    return (int) current_window;	
};
*/


/*
 *******************************************************
 * RegisterWindow: (API)
 *     Registrando uma janela numa lista de janelas.
 */

// Belongs to kgws.

int RegisterWindow (struct window_d *window){

    struct window_d *Empty; 
    int Offset = 0; 


    debug_print ("RegisterWindow: [API]\n");

    if ( (void *) window == NULL ){
        debug_print ("RegisterWindow: window\n");
        return (int) -1;
    }
    
    if ( window->used != TRUE || window->magic != 1234 )
    {
        debug_print ("RegisterWindow: window validation\n");
        return (int) -1;
    }


	// Contagem de janelas e limites.
	// (� diferente de id, pois id representa a posi��o
	// da janela na lista de janelas).

    // Global

    windows_count++;

    if ( windows_count >= WINDOW_COUNT_MAX ){
        panic ("RegisterWindow: windows_count\n");
    }

    // Create empty.

// Loop

get_next:

    // #bugbug
    // Infinity loop?

    // Search empty slot
    // Limite da lista, volta ao in�cio da lista.

    Offset++;

    if (Offset >= WINDOW_COUNT_MAX){
        Offset = 0;
    }

    // Get empty

    Empty = (void *) windowList[Offset]; 

    if ( (void *) Empty != NULL ){
        goto get_next;
    }else{

        // Coloca na lista.
        // Onde est� vazio, registra o ponteiro.

        windowList[Offset] = (unsigned long) window; 

        // Configura o id.

        window->id = (int) Offset;

        // ...
    }; 

    // ok
    return 0;
}


/*
 * windowShowWindowList:
 *     Show Window List
 *     Mostra a lista de janelas registradas. 
 */
 
//#debug
//usado para debug.
 
void windowShowWindowList (void){

	struct window_d *hWindow;
	struct window_d *hWnd;

	unsigned long left;
	unsigned long top;
	unsigned long width;
	unsigned long height;

    int i=0;
    
    
	// Se n�o estivermos em modo gr�fico, n�o h� o que mostrar.

    if ( VideoBlock.useGui != 1 )
        return;


	// A janela da �rea de trabalho.

    if ( (void *) gui->main == NULL)
    {
        return;

    }else{
        left   = gui->main->left;
        top    = gui->main->top;
        width  = gui->main->width;
        height = gui->main->height;

		//...
    };


	//@todo: Chamar m�todo.	
	//Cursor.

    CONSOLE_TTYS[fg_console].cursor_x = (left/8);
    CONSOLE_TTYS[fg_console].cursor_y = (top/8);  
    //set_up_cursor(0,10);


	//
	// Se estamos no modo gr�fico.
	//


	//
	// @todo: Usar esquema de cores padr�o.
	//

    if( VideoBlock.useGui == 1 )
    {
        //Parent window.
        //#warning: J� checamos isso anteriormente.

        if( (void *) gui->main == NULL){ return; }

        hWindow = (void *) CreateWindow ( 3, 0, VIEW_MAXIMIZED, "window-list", 
                               left, top, width, height, 
                               gui->main, 0, 
                               KERNEL_WINDOW_DEFAULT_CLIENTCOLOR, 
                               KERNEL_WINDOW_DEFAULT_BGCOLOR ); 

        if ( (void *) hWindow == NULL )
        {
            printf ("windowShowWindowList:\n");
            return;
 
        }else{
            RegisterWindow (hWindow);
            kgwmSetActiveWindow (hWindow);
            kgwmSetFocus (hWindow);
        };


		// Alterando as margens.
		// Essas margens s�o usadas pela fun��o printf.
		// Obs: As medidas s�o feitas em n�meros de caracteres.
		// Obs: @todo: Devemos usar aqui o ret�ngulo da �rea de cliente,
		// e n�o as margens da janela.
		// A estrutura de janela deve nos oferecer os valores para a m�trica do 
		// ret�ngulo da �rea de cliente.
		// Obs: @todo:Acho que essa n�o � a forma correta de configurar isso. Uma 
		//rotina deveria perceber as dimens�es da janela de do caractere e determinar
		//as margens.

		// #bugbug
		// Cuidado para n�o dividir por '0'.

        CONSOLE_TTYS[fg_console].cursor_left   = (hWindow->left/8);
        CONSOLE_TTYS[fg_console].cursor_top    = (hWindow->top/8) + 4;   //Queremos o in�cio da �rea de clente.
        CONSOLE_TTYS[fg_console].cursor_right  = CONSOLE_TTYS[fg_console].cursor_left + (width/8);
        CONSOLE_TTYS[fg_console].cursor_bottom = CONSOLE_TTYS[fg_console].cursor_top  + (height/8);

        // cursor (0, mas com margem nova).
        CONSOLE_TTYS[fg_console].cursor_x = CONSOLE_TTYS[fg_console].cursor_left; 
        CONSOLE_TTYS[fg_console].cursor_y = CONSOLE_TTYS[fg_console].cursor_top; 

        //Mostrando as informa��es de todas as janelas registradas.
        while( i < WINDOW_COUNT_MAX )
        {
	        hWnd = (void *) windowList[i];
		
		    
		    // @todo: BUGBUG.
		    // O valor de hWnd tem que estar num limite v�lido, n�o adianta apenas ser
		    // diferente de NULL.
		    
		
		    if( (void *) hWnd != NULL )
			{
	            printf ("i={%d} h={%x} dBuf={%x} Name={%s}\n", 
				    i, hWnd, hWnd->DedicatedBuffer, hWnd->name );
					
				//draw_text( hWindow, 8,  1*(400/16), COLOR_WINDOWTEXT, "F1 Help.");
	        }
			
		    i++;
	    };

        show_active_window();
        show_window_with_focus();
        kgwmSetFocus(hWindow);


        // Voltando a margem normal a margem
        CONSOLE_TTYS[fg_console].cursor_left   = (left/8); 
        CONSOLE_TTYS[fg_console].cursor_top    = (top/8);
        CONSOLE_TTYS[fg_console].cursor_right  = (width/8);
        CONSOLE_TTYS[fg_console].cursor_bottom = (height/8); 

        // cursor (0, mas com margem nova)
        CONSOLE_TTYS[fg_console].cursor_x = CONSOLE_TTYS[fg_console].cursor_left; 
        CONSOLE_TTYS[fg_console].cursor_y = CONSOLE_TTYS[fg_console].cursor_top;
        //set_up_cursor(g_cursor_left,g_cursor_top); 

		//StatusBar ( hWindow, "Esc=EXIT","Enter=?" );
		//Nothing.
    };

    // Continua ...

    //@todo; 
	//Aqui podemos dar refresh apenas na janela
	
	// #bugbug
	// Isso � lento. Podemos apenas efetura refresh em uma parte.
	
	refresh_screen ();
    
	//kgwmSetFocus(hWindow);
}


/*
 *********************************************************
 * redraw_window: (API)
 * 
 *  #todo: Isso deveria ficar no mesmo lugar que createw.
 * 
 *     (Em fase de desenvolvimento)
 *     Repinta uma janela de acordo com os par�metros na estrutura.
 *     Isso depende do tipo. Para alguns tipos o trabalho ser� f�cil. 
 *     Repinta uma janela v�lida. Tem que estar registrada.  
 *     O OBJETICO DESSA ROTINA DEVE SER APENAS REPINTAR 
 * UM RET�NGULO QUE FOI PINTADO DENTRO DA �REA DE CLIENTE 
 * DA JANELA QUE FOI PASSADA VIA ARGUMENTO. (isso para o caso 
 * de janelas do tipo overlapped)
 * 
 * Obs: Esse ret�ngulo deve pertencer a janela que foi passada 
 * por argumento. 
 * Na estrutura de ret�ngulo deve haver um ponteiro para 
 * a janela � qual ele pertence.
 * Tamb�m deve-se repintar os elementos incluidos na 
 * �rea de cliente pelo programa aplicativo.
 *
 * A estrutura de janela deve registrar todos os elementos 
 * que foram inclu�dos na hora da sua cria��o.
 * *****************
 *
 *     @todo: Mudar para windowRedrawWindow(.)
 *
 * Obs: Esse � o tipo de rotina que exige o recurso 
 * de buffer dedicado.??
 * Pois podemos redezenhar a janela em seu buffer dedicado e 
 * depois copiar o conte�do do buffer dedicado para o 
 * backbuffer... na hora de atualizar a tela
 * � so copiar o backbuffer no frontbuffer(LFB).
 *
 * Pois bem, essa janela vai ter muitos elementos para 
 * incluir isso leva muito tempo.
 * Devemos registrar na estrutura da janela todos os 
 * bot�es que ela tem na hora de cri�-la.
 * Todos os textos gravados nela... todas as cores etc..
 * Obs: Talvez essa rotina seja recursiva assim como 
 * a fun��o create window.
 *
 * 
 * >>> e se a janela for um bot�o ?? ...
 *     gostar�amos de mudar o status do bot�o e depois repint�-lo 
 **   Criar a fun��o que muda o status do bot�o 
 *    usar redraw_window para repintar uma janela do tipo bot�o. 
 *    assim poderemos setar o foco em janela e em bot�o.
 */
 
// int windowRedrawWindow(struct window_d *window) 

// window server stuff.

//#todo:
// chamar isso quando chamarmos raise_window.

// IN: redesenha a janela, mostra na tela ou n�o.

int redraw_window (struct window_d *window, unsigned long flags ){

    int Status=0; 
    unsigned long border_size = 0;
    unsigned long border_color = 0xC0C0C0;
    unsigned long __tmp_color=0;


    debug_print ("redraw_window: [API]\n");

    if ( (void *) window == NULL ){
        debug_print ("redraw_window: [FAIL] window\n");
        goto fail;
    }

    if ( window->used != TRUE || window->magic != 1234 )
    {
        debug_print ("redraw_window: [FAIL] window validation\n");
        goto fail;
    }

    // ...
 
	// Ok. 
	// Pelo jeito j� temos uma estrutura v�lida.

	//@todo: Pra que serva a flag redraw ??
	// � para indicar que repecisamos repintar. ??
	//if( window->redraw != 1 ){ return (int) 0;}
	
	
    // Minimized ? 
    // Se tiver minimizada, n�o precisa repintar.

    Status = (int) is_window_minimized (window);
    
    if (Status == TRUE){
        debug_print ("redraw_window: Minimized\n");
        goto done;
    }

	//E se ela estiver travada ??
	//O que significa travada?? n�o pode se mover??
	// ?? travada pra quem ??
	//if(window->locked == 1){}
	
	//@todo:
	//docked ?? num canto da tela.

	
	//Pintar ?? Onde ?? backbuffer ?? dedicated buffer ??
//drawWindow:	


	// *********************************
	// Devo lembrar que as caracter�sticas da janela est�o salvas 
	// na estrutura e s�o muitas caracter�sticas. 
	// N�o exige c�lculos, toda a m�trica j� est� pronta.
	// *********************************


	//Buffers ??
	//window->DedicatedBuffer
	//window->BackBuffer
	//window->FrontBuffer
	
	// Importante: 
	// Checando se o esquema de cores est� funcionando.
	// obs: Essa checagem est� se tornando repetitiva.

    if ((void *) CurrentColorScheme == NULL){
        panic ("redraw_window: CurrentColorScheme");
    } else {
        if ( CurrentColorScheme->used != TRUE || 
             CurrentColorScheme->magic != 1234 )
        {
            panic ("redraw_window: CurrentColorScheme validation\n");
        }
    };


	// Importante:
	// Agora, na hora de efetivamente redezenhar, 
	// podemos ver na estrutura quais s�o os elementos 
	// presentes nela. Lembrando que esses elementos podem ter 
	// sido modificados, ent�o o melhor lugar para pegar essas
	// informa��es � na estrutura mesmo, pois l� ser�o salvos 
	// as prefer�ncias atuais do usu�rio em rela��o a janela 
	// que deve ser repintada.
	// Ex: O usu�rio pode ter redimencionado ou arrastado 
	// elementos da janela, ou ocultado apenas. 
	// Afinal, feito escolhas ... 


//
// Redraw 
//


	// Cada estilo de design tem suas caracter�sticas,
	// essas caracter�sticas precisas ser registradas 
	// na estrutura de janela, bem como o pr�prio estilo 
	// de design.
	
//redrawBegin:

    // #bugbug
    if ( window->view == VIEW_NULL ){ 
        debug_print ("redraw_window: VIEW_NULL\n");
        goto fail; 
    }


    if ( window->type == WT_OVERLAPPED ){
        debug_print ("redraw_window: Overlapped\n");
    }

    // Shadow for overlapped windows only.
    // #bugbug
    // A sombra deve ter suas dimens�es registradas tamb�m.
    if (window->shadowUsed == TRUE)
    {
        if ( window->type == WT_OVERLAPPED )
        {
            // @todo: Adicionar a largura das bordas verticais 
			// e barra de rolagem se tiver.
			// @todo: Adicionar as larguras das 
			// bordas horizontais e da barra de t�tulos.
			// Cinza escuro.  CurrentColorScheme->elements[??] 
			// @TODO: criar elemento sombra no esquema. 

            if (window->focus == TRUE) { __tmp_color = xCOLOR_GRAY1; }
            if (window->focus == FALSE){ __tmp_color = xCOLOR_GRAY2; } 

            drawDataRectangle ( 
                window->left +1, 
                window->top  +1, 
                window->width  +2, 
                window->height +2, 
                __tmp_color ); 
        }
    }


    // Background for all types.
    if (window->backgroundUsed == TRUE){
        drawDataRectangle ( 
            window->left, 
            window->top, 
            window->width, 
            window->height, 
            window->bg_color ); 
    }


    // Border for some types only.
    // #importante:
    // devemos tratar a borda para cada tipo de janela individualmente.
    if ( window->borderUsed == TRUE )
    {
        debug_print ("redraw_window: [DEBUG] Border used\n");

        // Button
        if ( window->type == WT_BUTTON )
        {
            if ( window->focus == TRUE ){
                border_color = COLOR_BLUE;  border_size = 2;
            }else{
                border_color = 0xF4F7FC;    border_size = 1;
            };
        }

        // Editbox
        if ( window->type == WT_EDITBOX )
        {
            if ( window->focus == TRUE ){
                border_color = COLOR_BLUE;  border_size = 2;
            }else{
                border_color = 0xF4F7FC;    border_size = 1;
            };
        }

        // Overlapped 
        if ( window->type == WT_OVERLAPPED )
        {
            // Active?
            if (window->active == TRUE){ 
                debug_print ("redraw_window: Active\n");
                border_color = COLOR_BLUE;
                border_size = 2; 
                if ( window->focus == TRUE )
                {
                    border_color = COLOR_YELLOW; 
                    border_size = 3; 
                }
            }else{
                debug_print ("redraw_window: Not active\n");
                border_color = 0xF4F7FC;
                border_size = 1;
            };
        }

        // Simple
        if ( window->type == WT_SIMPLE )
        {
            if ( window->focus == TRUE ){
                border_color = COLOR_GRAY;  border_size = 2;
            }else{
                border_color = 0xF4F7FC;    border_size = 1;
            };
        }


		//board1, borda de cima e esquerda.    
		drawDataRectangle ( window->left, window->top, 
			window->width, border_size, border_color );
						   
	    drawDataRectangle ( window->left, window->top, 
			border_size, window->height, border_color );

	    //board2, borda direita e baixo.
	    drawDataRectangle ( window->left +window->width -1, window->top, 
		    border_size, window->height, border_color );
					   
	    drawDataRectangle ( window->left, window->top +window->height -1, 
			window->width, border_size, border_color );		
	};




    // T�tulo + borda (frame).
    // Repinta ret�ngulo e string para todos os tipos.
    if (window->titlebarUsed == 1)
    { 
		//#importante:
		//@todo:Ainda h� muito o que levar em considera��o 
		//na hora de repintar uma janela com rela��o 
		//ao fato de serem ativas ou n�o ou de terem o foco ou n�o.
 
        //no caso de janela m�e.
        //if (window->id == active_window){
			//window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar];
		//}else{
			//window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		//};
	
		//Focus.
		//Cores diferentes se tiver foco e se n�o tiver.
		//no caso de janelas filhas
		//if (window->id == window_with_focus){
		    //window->bg_color = CurrentColorScheme->elements[csiActiveWindowTitleBar]; 
		//}else{
			//window->bg_color = CurrentColorScheme->elements[csiInactiveWindowTitleBar]; 
		//};
		
		//@todo: String color.
		
		//@todo: String size.
		
		//@todo: Se tiver barra de rolagem, a largura deve ser maior.
		//if()
		
		//Rectangle and string.
		
        drawDataRectangle ( window->left, window->top, 
			window->width +1 +1, window->height +1 +1, 
			window->bg_color );

        // #bugbug
        // na m�quina real, falhou logo ap�s pintar a string da barra de t�tulos.
        // Vamos usar breakpoint at� acharmos o problema.

       // barra de t�tulos; 
       //todo: usar o esquema de cores.       
			if (window->focus == 1)
			{ __tmp_color = xCOLOR_GRAY1; }        // mais escuro
			if (window->focus == 0)
			{ __tmp_color = window->bg_color; }    // escolhida pelo aplicativo;
        
        drawDataRectangle ( 
            (window->left +2), (window->top +2), 
            window->width -4, 30, 
            __tmp_color );        


		//@todo: 
		//string da barra de t�tulos.
		//Se estivermos em full screen, n�o teremos string.

        draw_string ( 
            (window->left +8 +16 +8), (window->top +8 +4), 
            xCOLOR_GRAY7, window->name ); 


		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}
		
	    //Isso � um teste.
	    //A janela nem foi registrada ainda e j� estamos passando o handle
	    //via argumento.
		
	    //So criamos o bot�o na barra de t�tulos se tivermos uma barra de t�tulos.
		//ent�o esse � o lugar certo para essa rotina.
		
		// #todo: 
		// Se estivermos em full screen, n�o teremos bot�o.	
		/*
		if (window->minimizebuttonUsed == 1)
		{        
			if ( window->minimize != NULL )
			{
				if ( window->minimize->used == 1 && 
				     window->minimize->magic == 1234 )
				{
			        //draw_button ( window, "V", 
			        //    1, 0, 0, 
			        //    window->minimize->left, window->minimize->top, window->minimize->width, window->minimize->height, 
				    //    window->minimize->bg_color );
				}
				
			}
	    };
		*/
		
		/*
		if (window->maximizebuttonUsed == 1)
		{
			if ( window->maximize != NULL )
			{
				if ( window->maximize->used == 1 && 
				     window->maximize->magic == 1234 )
				{
			        //draw_button ( window, "^", 
			        //    1, 0, 0, 
			        //    window->maximize->left, window->maximize->top, window->maximize->width, window->maximize->height, 
				    //    window->maximize->bg_color );	
				}
				
			}					
		}
		*/
		
		// #todo: 
		// Se estivermos em full screen, n�o teremos bot�o.
	    
	    /*
		if (window->closebuttonUsed == 1)
		{			
			if( window->close != NULL )
			{
				if ( window->close->used == 1 && window->close->magic == 1234 )
				{
			        //draw_button ( window, "X", 
			        //    1, 0, 0, 
			        //    window->close->left, window->close->top, window->close->width, window->close->height, 
				    //    window->close->bg_color );
				}
				
			}		
	    };
	    */
	    				 
		//...
	};		
	

        //funcionou at� aqui na m�quina real.
		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}


    // Client Area. 
    // #bugbug
    // Precisamos repintar a client area, mas a rotina de resize
    // n�o modificou a client area. #todo.
    if (window->clientAreaUsed == 1)
    {
		// Obs: 
		// A Client Area � apenas um ret�ngulo ??

		// #todo: 
		// Passar a estrutura de janela.
		
		// #BUGBUG: 
		// ( N�O ) PODEMOS REALOCAR NOVAMENTE ... 
		// TEMOS QUE CHECAR SE A ESTRUTURA � V�LIDA...
		// SE TRATA APENAS DE UMA ESTRUTTURA DE RET�NGULO, 
		// N�O ESPERAMOS MUITOS PROBLEMAS.
		 
		// #bugbug
		// Existem janelas que n�o possuem reClient.
		// Elas n�o merecem falhar; 
		 
		//if ( (void *) window->rcClient == NULL )
		//{
			//panic ("redraw_window: rcClient \n");
			
		//}
			
		if ( (void *) window->rcClient != NULL )
		{		
			//conferir validade.
			if ( window->rcClient->used != 1 || 
			     window->rcClient->magic != 1234 )
			{
			    panic ("redraw_window: rcClient validation \n");
			}
			
			// Aqui devemos respeitar a forma que a �rea de cliente 
			// foi desenhada.
			// Mas todos os valores que precisamos est�o salvos na estrutura 
			// de �rea de cliente.	
			
            drawDataRectangle ( (unsigned long) window->rcClient->left, 
		        (unsigned long) window->rcClient->top, 
				(unsigned long) window->rcClient->width, 
				(unsigned long) window->rcClient->height, 
				(unsigned long) window->rcClient->bg_color );
		}
		//Nothing.
    }


	//status bar
    if (window->statusbarUsed == 1)
    {
	    if ( window->statusbar != NULL )
		{
			if ( window->statusbar->used == 1 && 
			     window->statusbar->magic == 1234 )
			{
		        drawDataRectangle ( window->statusbar->left, 
		            window->statusbar->top, 
			        window->statusbar->width -1, 
			        window->statusbar->height, 
			        window->statusbar->bg_color ); 

		        draw_string ( window->statusbar->left +8, 
		            window->statusbar->top +8, 
		            0x00333333, window->statusbar->name ); 
			}
		}
    }

       //funcionou ate aqui na maquina real
		//#debug
		//printf ("* real machine breakpoint");
		//refresh_screen();
		//while(1){}


	//
	// ## Button ##
	//
	
	// Vamos apenas repintar a janela tipo bot�o do mesmo jeito que repintamos
	// as outras e apenas atualizar a estrutura se for necess�rio.
	
	if ( window->type == WT_BUTTON )
	{
	    if ( window->isButton == 1 )
        {
			if ( (void *) window->button != NULL )
			{
				if ( window->button->used == 1 && 
				     window->button->magic == 1234 )
				{
					// #importante
				    // Redesenhar o bot�o com base nas informa��es da estrutura.    
					// #todo: Checar sytle state type 
					// #obs: Com focus a borda fica azul. 
					// O redraw pode ter vindo ap�s uma atualiza��o do bot�o.
					
					switch (window->button->state)
					{
						case BS_FOCUS:
						    window->button->border1 = COLOR_BLUE;
						    window->button->border2 = COLOR_BLUE; 
							break;
							
						case BS_PRESS:
						    window->button->border1 = 0x00404040;
						    window->button->border2 = 0x00303030; 
							break;
							
						case BS_DEFAULT:
						    window->button->border1 = 0x00303030;
						    window->button->border2 = 0x00404040; 
							break;
								
						case BS_DISABLED:
						    window->button->border1 = COLOR_GRAY;
						    window->button->border2 = COLOR_GRAY; 
						    window->button->color = COLOR_GRAY;
							break;
							
						case BS_HOVER:
							window->button->color = ( window->button->color - 20);
							break;
							
						//default:
						//	printf ("redraw button default\n");
							//break;
					};
					
	                //bg 
	                drawDataRectangle ( window->left, window->top, 
	                    window->button->width, window->button->height, 
	                    window->button->color );
					
					//board1, borda de cima e esquerda.
	                drawDataRectangle ( window->left, 
	                    window->top, 
	                    window->button->width, 1, 
	                    window->button->border1 );
		
	                drawDataRectangle ( window->left, 
	                    window->top, 
	                    1, window->button->height, 
	                    window->button->border1 );

	                //board2, borda direita e baixo.
	                drawDataRectangle ( window->left + window->button->width -1, 
	                    window->top, 
		                1, window->button->height, 
		                window->button->border2 );
					   
	                drawDataRectangle ( window->left, 
	                    window->top + window->button->height -1, 
		                window->button->width, 1, 
		                window->button->border2 );
				    
					//#todo: if aqui tem duas op��es de draw string.
                    if ( window->button->selected == 1 )
					{
					    draw_string ( window->left +8, 
					        window->top +8, 
			                COLOR_WHITE, window->button->string );
                    }
					
                    if ( window->button->selected == 0 )
                    {
                        draw_string ( 
                            (window->left +8), (window->top  +8), 
                            COLOR_WHITE, window->button->string );
                    }
				}
			}
		}
	};


    // Icon.
    if (window->type == WT_ICON)
    {
        bmpDisplayBMP ( shared_buffer_app_icon, 
            (window->left +8), (window->top +8) );
            
        draw_string ( 
            (window->left +8), (window->top +16 +8 +8), 
            xCOLOR_GRAY7, window->name );  
    }



    //#test
    //manda uma mensagem para thread atual
    //para ela mostrar os elementos da �rea de cliente.

    // isso ser� usado na fun��o update window.()
    /*
    // n�o podemos fazer isso para todos os tipos de janelas.
    // pois entraria num loop de recursividade.
    if ( window->type == WT_OVERLAPPED || window->type == WT_SIMPLE )
    {
		// Validade da thread.
        if ( (void *) window->control != NULL )
        {
			// Validade da thread.
			if ( window->control->used == 1 || 
			     window->control->magic == 1234 )
			{
			   // mandamos a mensagem
			   // o aplicativo decide o que fazer com ela.
			    window->control->window = window;
			    window->control->msg = MSG_PAINT;
			    window->control->long1 = 0;
			    window->control->long2 = 0;
			    window->control->newmessageFlag = 1;
			}
		}
    };
    */

	//
	// Outros elementos ainda n�o implementados ...
	//
	
	//checar na estrutura se essa janela tinha:
	//bot�es ?? bot�es de radio ?? textos ?? barras ??
	//etc ...


	// A rotina de repintura deve estar em sintonia com a rotina de pintura, elas 
	// devem ser semelhantes, porem a rotina de repintura deve levar em considera��o 
	// os elementos inclu�dos depois da cria��o da janela e que ficam gravados na 
	// estrutura. como por exemplo: arquivos abertos.

	
	//poderemos ter mais valores em flags no futuro.
		// #obs:
		// Quando uma rotina muda a posi��o da janela.
		// E em seguida repinta. Esse ser� o ret�ngulo que 
		// ser� mostrado no front buffer, mesmo se a rotina
		// de repintura falhar.
		
		// #test
		// Vamos mostrar um pouco mais.
		// #todo: Esse extra deve ser do tamanho da sombra.

    if ( flags == TRUE ){
        refresh_rectangle ( 
            window->left, 
            window->top, 
            window->width +4, 
            window->height +4 );
    }

    // ...

done:
    debug_print ("redraw_window: done\n");
    return 0; 
fail:
    debug_print ("redraw_window: fail\n");
    return (int) 1;
}


/*
 ******************************
 * redraw_screen:
 *
 *     Repinta todas as janelas com base na zorder.
 *     @todo: 
 * Obs: Ao repintar cada janela a rotina redraw_window dever� 
 * incluir todos os elementos da janela. 
 * Do mesmo jeito que o usu�rio modificou de acordo com suas prefer�ncias.
 */


// Vamos procurar na lista por ponteiros v�lidos.
// Repintaremos todas as janelas com ponteiros v�lidos. 

    // #todo
    // Repintar todas as janelas de baixo para cima
    // mas s� as que estiverem invalidadas.


// funcionou mas est� piscando ...
// como se estivesse pintando a mesma janela duas vezes.
// provavelmente a �ltima; 
 
int redraw_screen (void){

    struct window_d *__window;
    int RedrawStatus;    
    int z=0;



    for ( z=0; z < KGWS_ZORDER_MAX; z++ )
    {
        __window = (void *) Windows[z];

        if ( (void *) __window != NULL )
        {
            if ( __window->used == 1 && __window->magic == 1234 )
            {
                // Compara os �ndices.
                if ( __window->z != z ){
                    panic ("wm-redraw_screen: index\n");
                }

                //retira o foco.
                //kgwmKillFocus(__window);

                // durty.
                // se ela foi marcada como suja e precisa ser repintada.
                //if ( __window->dirty == 1 )
                //{
                    //Repinta uma janela.
                    RedrawStatus = (int) redraw_window (__window, 1);
                    if (RedrawStatus == 1){
                        panic ("redraw_screen: status\n");
                    }
                //}
            }
        };
    };


    // #obs
    // Se for terminar corretamente � porque 
    // repintamos tudo o que foi poss�vel.

done:
    return 0;
}


/*
 *********************************************
 * resize_window: (API)
 * 
 *     Change dimentions.
 */

int 
resize_window ( 
    struct window_d *window, 
    unsigned long cx, 
    unsigned long cy )
{

    debug_print ("resize_window: [API]\n");
 
 
    if ( (void *) window == NULL ){
		//message
	    return 1;    

    } else {	
	    
		//@todo: Checar limites.
	
	    // Ok mudamos as dimens�es da janela principal.
	    // Mas e se ela tiver uma janela client window?
        window->width = (unsigned long) cx;
        window->height = (unsigned long) cy;

        
        if (window->clientAreaUsed == 1)
        {
			if ( (void *) window->rcClient != NULL )
			{
				//validade da estrutura de ret�ngulo
				if ( window->rcClient->used == 1 &&
				     window->rcClient->magic == 1234 )
				{
					if ( window->type == WT_SIMPLE )
					{
					    window->rcClient->width = window->width;
					    window->rcClient->height = window->height;
					}
					
                    if ( window->type == WT_OVERLAPPED )
                    {
                        window->rcClient->width = (unsigned long) (window->width -1 -1);
                        window->rcClient->height = (unsigned long) (window->height -2 -32 -2 -24 -2);
			        }
			        
                    if ( window->type == WT_EDITBOX || 
                         window->type == WT_EDITBOX_MULTIPLE_LINES )
                    {
                        window->rcClient->width = (unsigned long) (window->width -1 -1);
                        window->rcClient->height = (unsigned long) (window->height -1 -1);
                    }
				} 
			}
        }
        
        if (window->statusbarUsed == 1)
        {
			if ( (void *) window->statusbar != NULL )
			{
			    window->statusbar->width = window->width;
			    window->statusbar->height = 25;
			    //window->statusbar->bg_color = COLOR_PINK;
			    
			    //ajuste
			    window->bottom = window->top + window->height;
			    window->statusbar->top = window->bottom -25-1; 
		    }
        }

        //validade da thread,
		if ( (void *) window->control != NULL )
        {
            if ( window->control->used == 1 || 
                 window->control->magic == 1234 )
			{
			    // mandamos a mensagem
			    // o aplicativo decide o que fazer com ela.
			    //window->control->window = window;
			    //window->control->msg = MSG_SIZE;
			    //window->control->long1 = 0;
			    //window->control->long2 = 0;
			    //window->control->newmessageFlag = 1;
			}
		}
	}


    return 0;
}


/*
 ****************************************************
 * replace_window: (API)
 *     Muda os valores do posicionamento da janela.
 */

int 
replace_window ( 
    struct window_d *window, 
    unsigned long x, 
    unsigned long y )
{


    debug_print ("replace_window: [API]\n");

    if ( (void *) window == NULL ){
        debug_print ("replace_window: window\n");
        return -1;
    } 

    // #todo
    // Check lock
    
    //if ( window->locked == TRUE ){
    //    debug_print ("replace_window: Locked\n")
    //    return -1;
    //}


    // #todo: 
    // Checar limites.

    window->left = (unsigned long) x;
    window->top  = (unsigned long) y; 


    // Client area

    if (window->clientAreaUsed == TRUE)
    {
        if ( (void *) window->rcClient != NULL )
        {
            if ( window->rcClient->used  == TRUE &&
                 window->rcClient->magic == 1234 )
            {
                    // #bugbug
                    // Isso precisa estar em conformidade
                    // com os padr�es encontrados em createw.c

                    if ( window->type == WT_SIMPLE )
                    {
                        window->rcClient->left = (unsigned long) (window->left);
                        window->rcClient->top  = (unsigned long) (window->top);
                    }

                    if ( window->type == WT_OVERLAPPED )
                    {
                        window->rcClient->left = (unsigned long) (window->left +1);
                        window->rcClient->top  = (unsigned long) (window->top  +2 +32 +2);
                    }

                    if ( window->type == WT_EDITBOX || 
                         window->type == WT_EDITBOX_MULTIPLE_LINES )
                    {
                        window->rcClient->left = (unsigned long) (window->left +1);
                        window->rcClient->top  = (unsigned long) (window->top  +1);
                    }
            } 
        }
    }


    // Status bar

    if (window->statusbarUsed == TRUE)
    {
        if ( (void *) window->statusbar != NULL )
        {
            window->bottom = (window->top + window->height); 

            window->statusbar->left = (window->left +1);
            window->statusbar->top  = (window->bottom -25 -1);
        }
    }


// done:
    return 0;
}


/*
 ************************************************
 * is_window_full:
 *     Checar se est� no modo tela cheia.
 *
 *     retorno 1, modo tela cheia.
 * @todo: Rever o retorno.
 */

// Checa modo tela cheia.

int is_window_full ( struct window_d *window ){

    if ( (void *) window == NULL ){
        return FALSE;
    } else {
        if ( window->view == VIEW_FULL ){ return (int) TRUE; }
    };

    return FALSE;
}


/*
 ********************************************
 * is_window_maximized:
 *     Checar se est� no modo maximizado.
 *
 *     retorno 1, modo maximizada.
 * @todo: Rever o retorno.
 */

// Checa se j� est� maximizada.

int is_window_maximized (struct window_d *window){

    if ( (void *) window == NULL ){
        return FALSE;
    } else {
        if ( window->view == VIEW_MAXIMIZED ){ return (int) TRUE; }
    };

    return FALSE;
}


// Checa se j� est� minimizada.

int is_window_minimized (struct window_d *window){

    if ( (void *) window == NULL){
        return FALSE;
    } else {
        if ( window->view == VIEW_MINIMIZED ){ return (int) TRUE; }
    };

    return FALSE;
}


/*
 ***********************************************
 * CloseWindow: (API)
 * 
 *     Fecha uma janela. (Destr�i).
 *
 *     Obs: O ato de fechar uma janela est� associado
 *          ao ato de encerrar um processo. 
 * Mas nem sempre.
 * *Importante: @todo: Fechar  ajanela qui deve 
 * signifcicar apenas sinalizar para o GC que ele 
 * pode atuar sobre a estrutura.
 */

void CloseWindow ( struct window_d *window ){

    int Offset=0;
    int z=0;


    debug_print ("CloseWindow: [API]\n");

    if ( (void *) window == NULL ){
        debug_print("Closewindow: window\n");
        return; 
    }

    // #todo
    //if (window->locked == TRUE){
    //    debug_print("Closewindow: Locked\n");
    //    return; 
    //}


    // Obs:
    // N�o fechamos a janela principal.
    // Se for a janela principal.
    // if( (void*) window == (void*) gui->main ){
    //     return;
    // }

    // ...

    // Focus
    // Se temos uma janela m�e v�lida. Ela herda o foco.
    // #bugbug
    // Isso � meio recursivo.
    // O foco n�o vai acabar parando na root window?

    kgwmKillFocus (window);

    if ( (void *) window->parent != NULL )
    {
        if ( window->parent->used  == TRUE && 
             window->parent->magic == 1234 )
        {
            set_current_window (window->parent);

            kgwmSetActiveWindow (window->parent);
            kgwmSetFocus (window->parent);
        } 
    }

    // Devemos retirar a janela fechada da zorder list. 

    z = (int) window->z;

    if ( z >= 0 && z < KGWS_ZORDER_MAX )
    {
        Windows[z] = (unsigned long) 0;

        zorderCounter--;
        if (zorderCounter < 0 ){ zorderCounter = 0; }
    }

    // Sinaliza para o GC.
    // Com essas flags o GC poder�
    // liberar essa mem�ria para uso futuro.
    // #todo:
    // Usar um free();
    // Ou free_object();

    window->used  = WINDOW_GC;      // 216;
    window->magic = WINDOW_CLOSED;  // 4321;

    // ...
}

/*
int free_window_object( struct window_d *window );
int free_window_object( struct window_d *window )
{
	// #todo
	// Checar a estrutura de objecto
	// e marcar para o GC liberar para uso futuro.

	return 0;
}
*/

/*
 ******************************************************
 * DestroyWindow:
 *     Destr�i uma janela.
 *
 *     Obs: Quando a janela � do tipo Overlapped,
 *          destruir janela significa tamb�m fechar 
 *          o processo e as threads associados � ela.
 *
 *          Pois Overlapped � o tipo de janela usado
 *          como janela principal de um processo.
 *   Obs: Apenas sinalizaremos para o GC.
 */
 
// wm.
 
void DestroyWindow ( struct window_d *window )
{
    CloseWindow (window);
}


/*
 * kgwmGetActiveWindow:
 *     Obtem o id da janela ativa.
 */
 
void *kgwmGetActiveWindow (void)
{
    struct window_d  *aWindow;


    if (active_window < 0){
        debug_print ("kgwmGetActiveWindow: [FIXME] active_window underflow\n");
        return NULL;
    }

    if (active_window >= WINDOW_COUNT_MAX ){
        debug_print ("kgwmGetActiveWindow: [FIXME] active_window overflow\n");
        return NULL;
    }

    aWindow = (struct window_d *) windowList[active_window];

    if ( (void*) aWindow == NULL ){
        debug_print ("kgwmGetActiveWindow: [FIXME] Invalid pointer\n");
        return NULL;
    }
    
    if ( aWindow->used != TRUE || aWindow->magic != 1234 ){
        debug_print ("kgwmGetActiveWindow: [FIXME] validation\n");
        return NULL;
    }

    return (struct window_d *) aWindow;
}



/*
 ****************************************************
 * kgwmSetActiveWindow:
 *     Seleciona qual ser� a janela ativa.
 *     Ativa uma janela.
 */

// #todo
// change to 'int'

void kgwmSetActiveWindow (struct window_d *window)
{

    debug_print ("kgwmSetActiveWindow: [FIXME] \n");

    if ( (void*) window == NULL )
    {
        panic("kgwmSetActiveWindow: window\n");
    }
    
    if (window->used != TRUE || window->magic != 1234 )
    {
        panic ("kgwmSetActiveWindow: window validation\n");
    }

    // Change the active window.
    
    active_window = (int) window->id;

    debug_print ("kgwmSetActiveWindow: done\n");

    return;
}


/*
 * show_active_window:
 *     Mostra o id da janela ativa.
 *     @todo: Mudar o nome para windowShowActiveWindowId() 
 */

//#debug 

void show_active_window (void)
{
    printf ("ActiveWindow=%d \n", (int) active_window);
}


/*
 * show_window_with_focus:
 *     Mostra o id da janela com o foco de entrada..
 */

//#debug 

void show_window_with_focus (void)
{
    printf ("wwf=%d \n", (int) window_with_focus );
}


/*
 * CloseActiveWindow:
 *     Fecha a janela ativa.
 */

void CloseActiveWindow (void)
{

    debug_print ("CloseActiveWindow: [FIXME] \n");


    // DestroyWindow ( kgwmGetActiveWindow() );

    return;
}

void windowBlockFocus (void)
{
    gFocusBlocked = (int) TRUE;
}


void windowUnblockFocus (void)
{
    gFocusBlocked = (int) FALSE;
}


/*
 *****************************************************
 * kgwmSetFocus: (API)
 * 
 *     + Seta o foco em uma janela.
 *     + Configura o cursor.
 *     + Reinicia um buffer.
 *           
 * @todo:
 *     Tentando configurar o cursor dentro da �rea de cliente.
 *     Se a janela for o frame de aplicativo, ent�o o cursor 
 *     fica dentro dos limites da �rea de cliente, mesmo que esteja 
 * em full screen.
 *     Se a janela for um editbox, faz mais sentido ainda que 
 * o cursor fique dentro dos limites da �rea de cliente.
 *
 * #importante:
 *  Isso depende do tipo de janela. Se for um editbox tem que colocar o 
 * cursor no in�cio da �rea de edi��o.
 *
 * ...
 */

// #bugbug
// Revendo a quest�o de repintar a janela m�e quando se seta o foco. 

// #todo
// Setar o foco na janela e marcar a thread de controle associada a
// essa janela como foreground thread, para que ela receba input.

void kgwmSetFocus ( struct window_d *window )
{
    // priority stuff
    
    
    // Essa eh a thread associada com a janela que esta recebendo o foco.
    // Ela deve se tornar a foreground thread, para que receba o input.
    // Usamos a tid 'foreground_thread'
    struct thread_d *thread;

	// #debug
	// Testando com uma vers�o simplificada, pois isso est� falhado na m�quina real.
	// Sem foco do teclado n�o funciona.

    debug_print ("kgwmSetFocus: [API]\n");


    if ( (void *) window == NULL ){
        panic ("kgwmSetFocus: window\n");
    }else{
        if ( window->used != TRUE || window->magic != 1234 ){
            panic("kgwmSetFocus: Validation");
        }

        // #todo
        // process ad thread
        
        // foreground_process = (int) 
        // foreground_thread  = (int)
        
        
        window_with_focus = (int) window->id;
        
        window->focus = TRUE; 

        WindowWithFocus = (void *) window;
    };


    // Process and thread.

    //if ( (void *) window->control == NULL )
        //panic(?)


    // Let's check the validation of the control thread
    // and send it's pointer to the current TTY.
    // #bugbug: For now we're gonna setup only the virtual console. 
    // It's because it is easier to do this right now.
    // Focus, priority and quantum.
    if ( (void *) window->control != NULL )
    {
        
        
        // Focus!!
        // Who can read the input.
        
        active_process = window->control->ownerPID; 
        active_thread  = window->control->tid;
        
        foreground_process = active_process;
        foreground_thread  = active_thread;
      
      
        // Current virtual console.
        CONSOLE_TTYS[fg_console].control = (struct thread_d *) window->control;
      
        // ...
      
        // priority
        set_thread_priority ( 
            (struct thread_d *) window->control,
            PRIORITY_MAX );
    }
}


/*
 ***********************************
 * kgwmGetFocus: (API)
 *     Pega o ponteiro para a estrutura da janela com o foco de entrada.
 */

void *kgwmGetFocus (void)
{
    struct window_d *w;


    debug_print ("kgwmGetFocus: [API]\n");

    if ( window_with_focus < 0 ){
        debug_print("kgwmGetFocus: [FAIL] window_with_focus underflow\n");
        return NULL;
    }

    if ( window_with_focus >= WINDOW_COUNT_MAX ){
        debug_print("kgwmGetFocus: [FAIL] window_with_focus overflow\n");
        return NULL;
    }

    w = (struct window_d *) windowList[window_with_focus];

    if ( (void *) w == NULL ){
        debug_print("kgwmGetFocus: [FAIL] Invalid pointer\n");
        return NULL;
    }

    if ( w->used != TRUE || w->magic != 1234){
        debug_print("kgwmGetFocus: [FAIL] validation\n");
        return NULL;
    }

    return (struct window_d *) w;
}


/*
 ***********************************************
 * windowGetForegroundWindow:
 *     Recupera o handle da janela que o usu�rio est� trabalhando, ou seja, 
 * a janela em primeiro plano.
 */

// wm.

void *windowGetForegroundWindow (void)
{
    return (void *) windowList[window_with_focus];
}


/*
 **********************************************
 * windowSetForegroundWindow:
 *     Coloca uma janela em primeiro plano para o usu�rio trabalhar nela.
 *     @todo: aumentar aprioridade da thread que 
 * configurou a janela de primeiro plano.
 *     +o input do teclado deve vi pra essa janela.
 *     +modifica��es visuais deve ocorrer na janela 
 * que est� em primeiro plano, para 
 * que ela se destaque.
 * retorno: 0=OK  , 1=error.
 */

//wm

//#obs
// raise_window deve chamar isso e depois repintar e mostrar.

int windowSetForegroundWindow ( struct window_d *window ){


	if ( (void *) window == NULL ){
		printf ("windowSetForegroundWindow: window\n");
		goto fail;
	}else{

        kgwmSetFocus (window);
        kgwmSetActiveWindow (window);
		// ...
	};

	// #todo: 
	// Aumentar a prioridade da thread.

done:
    return 0;
fail:
    return (int) 1;
}


// Coloca a janela acima das outras.
int raise_window ( struct window_d *window ){

    struct window_d *parent;

    if ( (void *) window == NULL ){
		//message
        return -1;
    }else{

      // #bugbug
      // a janela filha nunca � a janela ativa.
      // somente se a janela m�e � a gui->main
      // Uma janela filha pode ser a janela com o foco de entrada.

      //se ja somos a janela filha com o foco de entrada,
      //ent�o n�o precisamos fazer coisa alguma.
      if ( window->parent->child_with_focus == window )
      {
      }
      
      parent = window->parent;
      parent->child_with_focus = window;

      windowSetForegroundWindow (window);

       //redesenha e mostra.
      redraw_window (window,1);
	};

    return 0;
}


/*
 ********************************
 * windowSwitchFocus:
 * 
 *     Muda a janela que est� com foco de entrada.
 *     @todo: Selecionar a janela indicada como next 
 * na estrutura.
 *     Em cada momento o foco est� em uma lista de janelas
 * e quando trocamos o foco, percorremos a lista atual.
 * mas a lista muda dependendo do ambeinte gr�fico que 
 * estamos.
 * Muda quando trocamos a janela ativa, muda quando trocamos o desktop.
 */

void windowSwitchFocus (void){

	int Max=0;
	int CurrentID=0;
	int NextID=0;

    struct window_d *window;
    struct window_d *next;
	//...

	//Max e Current.
	
	Max = (int) windows_count;
	CurrentID = (int) window_with_focus;	//salva
	

	// @todo: 
	// Essa rotina precisa ser refeita. 
	// Seguindo uma lista linkada de janelas.

	
	window = (void *) windowList[window_with_focus];	
	
	if ( (void *) window == NULL )
	{
		printf("windowSwitchFocus: window\n");
	    goto fail; 

	}else{

        kgwmKillFocus (window);  

		//Se a pr�xima janela � v�lida.
		if( (void*) window->next != NULL )
		{
			//Get next.
		    next = (void*) window->next;  
		   	    
			//Current id.	
		    NextID = (int) next->id;     
		   
		    //Se estiver dentro dos limites usaremos a pr�xima.
		    if(NextID > 0 && NextID < Max){
		        window_with_focus = (int) NextID;
	        }else{
			    window_with_focus = (int) CurrentID;	
			}

		    window = (void*) windowList[window_with_focus];			
            kgwmSetFocus(window);
			goto done;
		}else{
			
		    window_with_focus = (int) CurrentID;	
		    window = (void*) windowList[window_with_focus];			
            kgwmSetFocus(window);
            goto done; 
            
		};
		
		//Fail.
	};
	
	//Nothing.
	
done:

    redraw_window ( window, 1 );
	
    // #debug  
	// #todo: Criar mecanismo melhor pra mostrar a janela com o foco.
	
    //printf("Focus={%d}\n", (int) window_with_focus);
	//refresh_screen();
	
	return;

fail:
    return;
}


/*
 *************************************
 * kgwmKillFocus: (API)
 *     Uma janela perde o foco.
 */

void kgwmKillFocus ( struct window_d *window )
{

    debug_print ("kgwmKillFocus: [API] [FIXME] \n");

    if ( (void *) window == NULL ){
        debug_print ("kgwmKillFocus: window\n");
        goto fail; 
    }

    if ( window->used != TRUE || window->magic != 1234 )
    {
        debug_print ("kgwmKillFocus: window validation\n");
        goto fail; 
    }

    window->focus = FALSE;
        
    // CUIDADO!
    //window_with_focus = 0;
    //WindowWithFocus = NULL;
    //foreground_process = 0;
    //foreground_thread = 0;

done:
    return;
fail:
    return;
}


/*
 ******************************************
 * MinimizeWindow: (API)
 *     Minimiza uma janela.
 */

void MinimizeWindow (struct window_d *window){

    int Status = FALSE;


    debug_print ("MinimizeWindow: (API)\n");

    if ( (void *) window == NULL ){
        printf ("MinimizeWindow: window\n");
        return;
    }

    if ( window->used != TRUE || window->magic != 1234 )
    {
        printf ("MinimizeWindow: window validation\n");
        return;
    }

    // Get status
    
    Status = (int) is_window_minimized (window);

    if (Status == TRUE){
        printf ("MinimizeWindow: Status\n");
        return;
    }

    kgwmKillFocus (window);

    window->view = (int) VIEW_MINIMIZED;
}


/*
 **************************************
 * MaximizeWindow: (API)
 * 
 *     Maximiza uma janela.
 */

void MaximizeWindow (struct window_d *window){

    int Status=0;
    int RedrawAndShow = TRUE;


    debug_print ("MaximizeWindow: [API]\n");

    if ( (void *) window == NULL){
        printf ("MaximizeWindow: window\n");
        goto fail; 
    }

    if ( window->used != TRUE || window->magic != 1234 )
    {
        printf ("MaximizeWindow: window validation\n");
        return;
    }

    Status = (int) is_window_maximized(window);

    if (Status == TRUE){
        printf ("MaximizeWindow: Status\n");
        goto fail; 
    }

    if ( (void *) gui == NULL){
        panic ("MaximizeWindow: gui\n");
    }

    // Redimensionar ao tamanho da janela main.
    // A janela main eh o desktop menos a barra.

    if ( gui->main != NULL )
    {
        window->left   = gui->main->left;
        window->top    = gui->main->top;
        window->width  = gui->main->width;
        window->height = gui->main->height;
    }

done:

    kgwmSetActiveWindow (window);
    kgwmSetFocus (window);
    window->view = (int) VIEW_MAXIMIZED;

    if (RedrawAndShow == TRUE){
        redraw_window(window,TRUE);
    }

    return;

fail:
    return;
}


/*
 *************************************************
 * init_window_manager:
 *     Inicializa o gerenciamento de janelas.
 *     @todo windowmanagerInit()
 */

int init_window_manager (void)
{
    debug_print ("init_window_manager: deprecated\n");
    return 0;
}


/*
 *****************************************
 * init_windows:
 *     Inicializa a lista de janelas.
 *     Inicializa globais relativas � janelas.
 */
 
int init_windows (void){

	// #debug
	// Inicializa a lista de janelas.
	// printf("init_windows:\n");  	

    int Offset = 0;
    int k = 0;


    debug_print("init_windows:\n");

    for ( Offset=0; Offset < WINDOW_COUNT_MAX; Offset++ )
    {
        windowList[Offset] = (unsigned long) 0;
    }


	windows_count = 0;

    // Set current.
    set_current_window(NULL);



	// # cursor #
	// Desabilitar o cursor de textos.
	// Cada aplicativo habilita quando for usar.

    timerDisableTextCursor();


	//
    // Set fonts. @todo: Criar uma estrutura para 
	// caracter�sticas do char.
	//
		
	// 8x8 
	// 0x000FFA6E;    //ROM.
	g8x8fontAddress  = (unsigned long) BIOSFONT8X8; 
	//g8x16fontAddress = (unsigned long) 0x000FFA6E; 
	//...
	
		//#bugbug: 
		//Na verdade window.c n�o tem acesso a essa vari�vel,
		//� preciso chamar o servidor atrav�s de um m�todo para 
        //configur�-la.
        //@todo: gwsSetCurrentFontAddress( g8x8fontAddress );
	gws_currentfont_address = (unsigned long) g8x8fontAddress;
	
	//
	// Char parameters.
	//
	
	//@todo: Create SetCharParam(.,.)

    gcharWidth  = (int) 8;
    gcharHeight = (int) 8;


	//...	

//
// == Global variables =========================
//


    //  Security
    
    current_usersession = 0;
    current_room        = 0;
    current_desktop     = 0;

    // usession_count = 0;  //??
    rooms_count = 0; 
    desktops_count = 0;


    current_window = 0;
    windows_count = 0;


	fullscreen_window = 0;
	active_window = 0;
	window_with_focus = 0;
	mouseover_window = 0;
	top_window = 0;
	editbox_window = 0;
	combobox_window = 0;
	terminal_window = 0;
    //Continua...		


    // Menu
    current_menu = 0;


//
// == Color scheme =======================
//

	//inicializando os esquemas de cores.
	//@todo: Isso poderia ter um argumento, selecionado entre o 
	//tipo 1 e 2, humility e pride.
	//ColorSchemeHumility ou ColorSchemePride

    windowInitializeColorSchemeSupport();

	//input buffer support ??
	//output buffer support ??
	
	
	//inicializar as estruturas de backbuffer e frontbuffer.
	
//
// Backbuffer support. (espelho da mem�ria de video)
//


//BackBufferSupport:

    BackBufferInfo = (void *) kmalloc ( sizeof(struct backbufferinfo_d) );

    if ( (void *) BackBufferInfo == NULL ){
        debug_print("init_windows: BackBufferInfo\n");
        //goto fail;

    }else{

        BackBufferInfo->used  = 1;
        BackBufferInfo->magic = 1234;
        //BackBufferInfo->start = ?
        //BackBufferInfo->end = ?
        //BackBufferInfo->size = ?
        //...		
    };



//
// Frontbuffer support. (mem�ria de v�deo)
//


//FrontBufferSupport:

    FrontBufferInfo = (void *) kmalloc ( sizeof(struct frontbufferinfo_d) );
    
    if ( (void *) FrontBufferInfo == NULL ){
        debug_print("init_windows: FrontBufferInfo\n");
        //goto fail;

    }else{

		//Algumas informa��es foram enviadas pelo boot loader.
	    FrontBufferInfo->used = 1;
        FrontBufferInfo->magic = 1234;
        //FrontBufferInfo->start = ?
        //FrontBufferInfo->end = ?
        //FrontBufferInfo->size = ?	

        //FrontBufferInfo->width = ?	
        //FrontBufferInfo->height = ?	
        //FrontBufferInfo->bpp = ?			
		
        //... 

        //?? terminal ??		
	};		

	
	/*
	  test - testando queue para teclado.
	 ************************************
	 */
	 
	//inicializando a fila do sistema usada pelo teclado. 
	//initializeQueue(&myQueue); 
	 
	 
	keyboard_message_head = 0;
    keyboard_message_tail = 0;
	
    //inicializando fila de teclado.


	keybuffer_tail = 0;
	keybuffer_head = 0;


	// #importante:
	// Nesse momento n�o existem estruturas prontas para carregar arquivos.
   		
	//
	// Continua ...
	//


done:
    return 0;
fail:
    return (int) 1;
}


// Pegando a z-order de uma janela.

int get_zorder ( struct window_d *window )
{

    if ( (void *) window != NULL )
    { 
        return (int) -1;
    }

    if ( window->used != TRUE || window->magic != 1234 )
    { 
        return (int) -1;
    }

    return (int) window->z; 
}


	//refletindo ??
    //se um argumento for passado teremos que examinar a ordem das janelas filhas.	
	//sendo assim cada estrutura de janela precisa de uma lista de janelas filhas 
	// e a zorder delas ... 
	//
	//se nenhuma janela for passada como argumento 
	// teremos que olhar as janelas filhas da janela principal que � a gui->main.
	//a janela gui->main pode ser a janela mae de todas as outras ...
	// sendo a janela gui->main a janela principal do processo kernel.
	//o processo explorador de arquivos poder� ser o processo que 
	//seja a m�e de todos os aplicativos de usu�rio ... ent�o a 
	//z order na estrutura da janela desse aplicativo indica a ordem 
	//das janelas principais dos seus processos filhos ...
	//??
	// Uma estrutura de janela pode ter uma vari�vel top_window indicando o id 
	// da janela que est� no topo da z-order da janela.


// Pegando a o ponteiro da janela que est� 
// no topo da lista

struct window_d *getTopWindow (void)
{
    return (struct window_d *) Windows[KGWS_ZORDER_TOP];
}


// Obtendo a janela de n�vel mais alto.
int get_top_window (void)
{
    return (int) top_window;
}


// Setando a top window.
void set_top_window (int id)
{
    struct window_d  *w;

    // #bugbug
    // Check limits

    if ( id<0 || id>=WINDOW_COUNT_MAX)
        return;

    w = (struct window_d *) windowList[id];
    
    if ( (void *) w != NULL ){
        return;
    }
    
    if ( w->used != TRUE || w->magic != 1234 )
    {
        return;
    }

    top_window = (int) id;

    Windows[KGWS_ZORDER_TOP] = (unsigned long) w;    
}


// Fecha a janela ativa.

void closeActiveWindow (void)
{
    CloseActiveWindow();
}


//encontrando um slot livre na z-order global de 
//overlapped windows.

int z_order_get_free_slot (void){

    struct window_d *zWindow;
    int z=0;
    int response;
 


	for( z=0; z < KGWS_ZORDER_MAX; z++ )
	{
	    zWindow = (void*) Windows[z];
        
		//Obtendo um espa�o vazio.
		//Se for NULL, ent�o n�o tinha um ponteiro no slot.
		if( (void*) zWindow == NULL )
		{
			response = (int) z; 
			
			zorderCounter++;
			if (zorderCounter >= KGWS_ZORDER_MAX)
			{
				printf("z_order_get_free_slot: zorderCounter\n");
				goto fail;
			}

			goto done;
		}
		//Nothing
	};
	
fail:
    return (int) -1;
done:
    return (int) response;
}


/*
 ***********************************
 * windowLoadGramadoIcons:
 * 
 *     Carrega alguns �cones do sistema.
 *     It's a part of the window system's initialization.
 */

int windowLoadGramadoIcons (void){

    unsigned long fRet=0;

	//#debug
	//printf("windowLoadGramadoIcons:\n");

	//
	//  ## Icon support ##
	//

//iconSupport:

	// Carregando alguns �cones b�sicos usados pelo sistema.

	// ## size ##
	// Vamos carregar �cones pequenos.
	//@todo checar a validade dos ponteiros.


    // #bugbug
    // Size determinado, mas n�o sabemos o tamanho dos �cones.

    // 4 pages.
    // 16 KB ? Is it enough ?
    // Sim, os �cones que estamos usam possuem no m�ximo 2KB.
    // See: base/

    unsigned long tmp_size = (4*4096);

    // See: window.h
    shared_buffer_app_icon       = (void *) allocPages(4);
    shared_buffer_file_icon      = (void *) allocPages(4);
    shared_buffer_folder_icon    = (void *) allocPages(4);
    shared_buffer_terminal_icon  = (void *) allocPages(4);
    shared_buffer_cursor_icon    = (void *) allocPages(4);
    // ...


    if ( (void *) shared_buffer_app_icon == NULL ){
        panic ("windowLoadGramadoIcons: shared_buffer_app_icon\n");
    }

    if ( (void *) shared_buffer_file_icon == NULL ){
        panic ("windowLoadGramadoIcons: shared_buffer_file_icon\n");
    }

    if ( (void *) shared_buffer_folder_icon == NULL ){
        panic ("windowLoadGramadoIcons: shared_buffer_folder_icon\n");
    }

    if ( (void *) shared_buffer_terminal_icon == NULL ){
        panic ("windowLoadGramadoIcons: shared_buffer_terminal_icon\n");
    }

    if ( (void *) shared_buffer_cursor_icon == NULL ){
        panic ("windowLoadGramadoIcons: shared_buffer_cursor_icon\n");
    }

	//
	// Load
	//

    // app icon
    fRet = (unsigned long) fsLoadFile ( 
                               VOLUME1_FAT_ADDRESS,
                               VOLUME1_ROOTDIR_ADDRESS, 
                               FAT16_ROOT_ENTRIES,
                               "APP     BMP", 
                               (unsigned long) shared_buffer_app_icon,
                               tmp_size );
    if ( fRet != 0 ){
        panic ("windowLoadGramadoIcons: APP.BMP\n");
    }

    // file icon
    fRet = (unsigned long) fsLoadFile ( 
                               VOLUME1_FAT_ADDRESS,
                               VOLUME1_ROOTDIR_ADDRESS, 
                               FAT16_ROOT_ENTRIES, 
                               "FILE    BMP", 
                               (unsigned long) shared_buffer_file_icon,
                               tmp_size );
    if ( fRet != 0 ){
        panic ("windowLoadGramadoIcons: FILE.BMP\n");
    }

    // folder icon
    fRet = (unsigned long) fsLoadFile ( 
                               VOLUME1_FAT_ADDRESS, 
                               VOLUME1_ROOTDIR_ADDRESS, 
                               FAT16_ROOT_ENTRIES,
                               "FOLDER  BMP", 
                               (unsigned long) shared_buffer_folder_icon,
                               tmp_size );
    if ( fRet != 0 ){
        panic ("windowLoadGramadoIcons: FOLDER.BMP\n");
    }

    // terminal icon
    fRet = (unsigned long) fsLoadFile ( 
                               VOLUME1_FAT_ADDRESS, 
                               VOLUME1_ROOTDIR_ADDRESS, 
                               FAT16_ROOT_ENTRIES, 
                               "TERMINALBMP", 
                               (unsigned long) shared_buffer_terminal_icon,
                               tmp_size );
    if ( fRet != 0 ){
        panic ("windowLoadGramadoIcons: TERMINAL.BMP\n");
    }


    // cursor icon
    fRet = (unsigned long) fsLoadFile ( 
                               VOLUME1_FAT_ADDRESS, 
                               VOLUME1_ROOTDIR_ADDRESS, 
                               FAT16_ROOT_ENTRIES,
                               "CURSOR  BMP", 
                               (unsigned long) shared_buffer_cursor_icon,
                               tmp_size );
    if ( fRet != 0 ){
        panic ("windowLoadGramadoIcons: CURSOR.BMP\n");
    }

	// More ?

    return 0;
}


// get a shared buffer to a system icon.
// it is gonna be used by the window server.
void *ui_get_system_icon ( int n )
{
    if (n <= 0){ return NULL; }

    // See: window.h
    switch (n){

    case 1: return (void *) shared_buffer_app_icon;       break;
    case 2: return (void *) shared_buffer_file_icon;      break;
    case 3: return (void *) shared_buffer_folder_icon;    break;
    case 4: return (void *) shared_buffer_terminal_icon;  break;
    case 5: return (void *) shared_buffer_cursor_icon;    break;
    // ...
    };

    return NULL;
}


/*
 **************** 
 * top_at: 
 * 
 */
// pega a janela que est� mais ao topo da zorder e que
// corresponda �s cordenadas do mouse.
// retorna window id
int top_at ( int x, int y )
{
    int z = 0;
    int wID = -1;
    struct window_d *__last_found;
    struct window_d *tmp;

  
    //max 1024 janelas.
    for ( z=0; z<KGWS_ZORDER_MAX; z++ )
    {
        //pega a pr�xima na zorderlist;
        tmp = (struct window_d *) Windows[z];
        
        //check
        if ( (void *) tmp != NULL )
        {
            if ( tmp->used == 1 && tmp->magic == 1234 )
            {

                 // #importante
                 // Checando coordenadas.
                  if ( x > (tmp->left)  && 
                       x < (tmp->left + tmp->width)  && 
                       y > (tmp->top)  &&
                       y < (tmp->top + tmp->height)  )
                 {
					 //printf ("%d",z);
                      // salva essa.
                      __last_found = (struct window_d *) tmp;
                 }
            }
        }

    };

__found:

    window_mouse_over = __last_found->id;

    return (int) __last_found->id;
}


/*
 *******************************************************************
 * windowScan:
 *     Escaneia as janelas existentes procurando uma que contenha o 
 * posicionamento do cursor.
 * Ou seja, o posicionamento do cursor deve estar dentro da �rea da janela.
 *
 * Obs:
 *    A fun��o retorna na primeira janela encontrada.
 *    #todo: Mas dever�amos observar se essa janela possui janelas filhas, 
 * pois o ponteiro pode passar em cima de uma janela que est� dentro de 
 * outra janela.  
 */

// wm.
 
// #bugbug
// No caso dos bot�es o deslocamento encontrado � relativo
// � sua janela m�e. Ent�o tamb�m precisamos considerar
// o posicionamento da janela m�e ?? 

// IN: posicionamento do cursor.
// OUT: ID da janela.
 
int windowScan ( unsigned long x, unsigned long y ){

    struct window_d *w;
    int i = 0;
    int WID;


	// #bugbug
	// Vamos sondar toda a lista de janelas.
	// pois pode haver fragmenta��o na lista de janelas.
	// Ent�o mesmo com uma contagem baixa de janelas pode haver 
	// janelas v�lidas em posi��es altas.
	// Ent�o vamos sondar por toda a lista.


	// #BUGBUG
	// O problema nessa t�cnica s�o as sobreposi��o de janelas.
	// Quando uma janela est� dentro da outr, ent�o duas janelas
	// est�o �ptas a serem selecionadas.
	// Talvez devamos filtrar e s� aceitarmos sondar procurando 
	// por controles.


    for ( i=0; i < WINDOW_COUNT_MAX; i++ )
    {
        w = (struct window_d *) windowList[i];

        if ( (void *) w != NULL )
        {
            if ( w->used == TRUE && w->magic == 1234 )
            {

				// #bugbug
				// Precisamos considerar o deslocamento da janela m�e
				// para sabermos seu posicionamaento em rela��o � tela.

				// Para o botao vamos considerar o deslocamento
				// da janela mae.
				// Dentro da �rea da janela.

				// >> BUTTON or EDITBOX <<

                if ( w->type == WT_BUTTON || w->type == WT_EDITBOX )
                {
                    if ( (void *) w->parent == NULL )
                    {
                        panic ("windowScan: parent\n");
                    }

			        //printf ("X=%d Y=%d l=%d t=%d w=%d h=%d \n",
			            //x, y, w->left, w->top, w->width, w->height);

                    if ( x > (w->left)  && 
                         x < (w->left + w->width)  && 
                         y > (w->top)  &&
                         y < (w->top + w->height)  )
                    {

                        WID = w->id;
                        window_mouse_over = WID;

                        return (int) WID;
                    }
                }



				// Dentro da �rea da janela.
				/*
				if ( x > w->left   && 
					 x < w->right  && 
				     y > w->top    &&
				     y < w->bottom )
				{
					
					// #todo
					// Aqui precisamos de um tratamento diferenciado para cada um dos tipos.
					// Vamos come�ar com os bot�es.
					// Nesse momento cada um pode ter um compotamento diferente.
					// Pega o ID.
					
					if ( w->type == WT_BUTTON )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					if ( w->type == WT_EDITBOX )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					// ...
				}; 
				*/
            }
        }
    }; //for

    return (int) -1;
}


int windowOverLappedScan ( unsigned long x, unsigned long y ){

    struct window_d *w;
    int i = 0;
    int WID=0;


	// #bugbug
	// Vamos sondar toda a lista de janelas.
	// pois pode haver fragmenta��o na lista de janelas.
	// Ent�o mesmo com uma contagem baixa de janelas pode haver 
	// janelas v�lidas em posi��es altas.
	// Ent�o vamos sondar por toda a lista.


	// #BUGBUG
	// O problema nessa t�cnica s�o as sobreposi��o de janelas.
	// Quando uma janela est� dentro da outr, ent�o duas janelas
	// est�o �ptas a serem selecionadas.
	// Talvez devamos filtrar e s� aceitarmos sondar procurando 
	// por controles.


    for ( i=0; i < WINDOW_COUNT_MAX; i++ )
    {
        w = (struct window_d *) windowList[i];

        if ( (void *) w != NULL )
        {
            if ( w->used == TRUE && w->magic == 1234 )
            {

				// #bugbug
				// Precisamos considerar o deslocamento da janela m�e
				// para sabermos seu posicionamaento em rela��o � tela.

				// Para o botao vamos considerar o deslocamento
				// da janela mae.
				// Dentro da �rea da janela.

				// >> BUTTON or EDITBOX <<

                if ( w->type == WT_OVERLAPPED )
                {
                    if ( (void *) w->parent == NULL )
                    {
                        panic ("windowScan: parent\n");
                    }

                    //drawDataRectangle ( w->left, w->top, 
			        //    w->width, w->height, COLOR_RED );
			        //refresh_rectangle ( w->left, w->top, w->width, w->height );     
			        
			       // printf ("X=%d Y=%d l=%d t=%d w=%d h=%d \n",
			          //  x, y, w->left, w->top, w->width, w->height);
			    
                    if ( x > (w->left)  && 
                         x < (w->left + w->width)  && 
                         y > (w->top)  &&
                         y < (w->top + w->height)  )
                    {
                        //printf ("o ");
                        WID = w->id;
                        window_mouse_over = WID;

                        return (int) WID;
                    }
                }


				// Dentro da �rea da janela.
				/*
				if ( x > w->left   && 
					 x < w->right  && 
				     y > w->top    &&
				     y < w->bottom )
				{
					
					// #todo
					// Aqui precisamos de um tratamento diferenciado para cada um dos tipos.
					// Vamos come�ar com os bot�es.
					// Nesse momento cada um pode ter um compotamento diferente.
					// Pega o ID.
					
					if ( w->type == WT_BUTTON )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					if ( w->type == WT_EDITBOX )
					{    
						WID = w->id;
					    window_mouse_over = w->id;    
						return (int) WID;
					}
					
					// ...
				}; 
				*/

            }
        }
    }; //for

    return (int) -1;
}


// Envia uma mensagem PAINT para o aplicativo atualizar 
// a �rea de trabalho.

    // #wrong
    // We need to put the message in the queue.

void windowUpdateWindow ( struct window_d *window )
{
    // bugbug: esse send est� falhando.
    //windowSendMessage ( (unsigned long) window, 
        //(unsigned long) MSG_PAINT, 
        //(unsigned long) 0, 
        //(unsigned long) 0 );
        
        
    //window = (struct window_d *) arg2;

    if ( (void *) window == NULL ){  return; }

    if ( window->type == WT_OVERLAPPED || window->type == WT_SIMPLE )
    {

        // Validade da thread.
        // mandamos a mensagem
        // o aplicativo decide o que fazer com ela.

        if ( (void *) window->control != NULL )
        {
            if ( window->control->used  == TRUE || 
                 window->control->magic == 1234 )
            {

                // Single kernel event.

                // #wrong
                // We need to put it in the queue.

                window->control->ke_window = window;
                window->control->ke_msg    = MSG_PAINT;
                window->control->ke_long1  = 0;
                window->control->ke_long2  = 0;

                window->control->ke_newmessageFlag = TRUE;
            }
        }
    }
}


/*
 ***************************************
 * windowSwitchFullScreen:
 *     Switch full screen support.
 */

//salvando os valores usados para configura o modo 
//full screen

unsigned long save_client_rect_left;
unsigned long save_client_rect_top;
unsigned long save_client_rect_width;
unsigned long save_client_rect_height;

int fsStatus;

// uma janela entra ou sai do modo full screen.
// #todo: isso pode ir pra outro lugar, provavelmente gws.

int windowSwitchFullScreen (void){

    struct window_d  *window;

	//1 = entrar ; 0 = sair.
	int flag;
	
	// #debug
	// printf ("windowSwitchFullScreen: Initializing ...\n");
	
	//#teste


    // Se a janela ativa j� � a fullscreen 
    // ent�o � hora de sair.

    if ( fsStatus == TRUE )
    {
        current_window    = active_window;
        window_with_focus = active_window;
        fullscreen_window = 0;		
        flag = FALSE;
        fsStatus = FALSE;
        goto doswitch;
    }

    // Caso contr�rio.
    // Selecionando a janela ativa para entrar em fullscreen.

    current_window    = active_window;
    window_with_focus = active_window;
    fullscreen_window = active_window;
    flag = TRUE; 
    fsStatus = TRUE;

doswitch:

	//
	// Window 
	//

    window = (struct window_d *) windowList[current_window];

    if ( (void *) window ==  NULL  ){
        printf("windowSwitchFullScreen: window\n");
        goto fail;
    }else{

		if ( window->used != TRUE || window->magic != 1234 )
		{
			printf("windowSwitchFullScreen: window validation\n");
			goto fail;
		}
		
		//validade da estrutura do ret�ngulo do cliente.
		if ( (void *) window->rcClient == NULL )
		{
			printf("windowSwitchFullScreen: rcClient\n");
			goto fail;
		}else{

			if( window->rcClient->used  != TRUE || 
			    window->rcClient->magic != 1234 )
			{
			    printf("windowSwitchFullScreen: rect validation\n");
			    goto fail;						
			}
			
				//entrar
			    if( flag == 1 )	
				{
					printf("flag1\n");
					refresh_screen();
					//salvamos o antigo ret�ngulo.
					save_client_rect_left   = window->rcClient->left;
					save_client_rect_top    = window->rcClient->top;
					save_client_rect_width  = window->rcClient->width;
					save_client_rect_height = window->rcClient->height;

				    window->rcClient->left   = (unsigned long) 0;
				    window->rcClient->top    = (unsigned long) 0;
				    window->rcClient->width  = (unsigned long) systemGetSystemMetrics( 1 ); //screen width. 1
				    window->rcClient->height = (unsigned long) systemGetSystemMetrics( 2 ); //screen height.2
			        
                    window->view = VIEW_FULL;
					
                    backgroundDraw( (unsigned long) window->rcClient->bg_color );  
					refresh_screen();
				}

				//sair
			    if( flag == 0 )	
				{
					printf("flag0\n");
					refresh_screen();
					//Nesse caso temos que recuperar os valores  
					//do ret�ngulo da �rea de cliente.
				    window->rcClient->left   = (unsigned long) save_client_rect_left;
				    window->rcClient->top    = (unsigned long) save_client_rect_top;
				    window->rcClient->width  = (unsigned long) save_client_rect_width; 
				    window->rcClient->height = (unsigned long) save_client_rect_height; 
			    
				    window->view = VIEW_NORMAL;
					
					//flag = 1 ( refresh rectangle.)
					redraw_window ( (struct window_d *) window, TRUE );
				}
			
		};
		
		//Isso tamb�m atualiza o cursor.
		//pode ser um problema quando sa�mos do modo fullscreen.

        kgwmSetFocus (window); 
	};

    ldisc_init_modifier_keys();
    ldisc_init_lock_keys();

done:
    printf("DONE\n");
    refresh_screen();
    
    return 0;

fail:
    printf ("FAIL\n");
    refresh_screen();

    return 1;
}


// #DEPRECATED: DELETE!
int 
scroll_client_window (struct window_d *window)
{
    debug_print("scroll_client_window: [DEPRECATED]\n");
    return 0;
}

//
// == Power Trio =====================================
//

// Initialize the structures used by the Power Trio window manager.
// Called by init_window_manager()

int powertrio_initialize(void)
{
    int i=0;
    struct powertrio_client_d  *Client;


    PowerTrio.initialized = FALSE;

    // Setup the desktop used by the Power Trio.
    // #todo: It needs to be a valid pointer.

    //if ( (void*) CurrentDesktop == NULL ){
    //    panic ("powertrio_initialize: CurrentDesktop\n");
    //}

    // PowerTrio.desktop = (struct desktop_d *) CurrentDesktop;


    // Creating clients

    for (i=0; i<3; i++){
        Client = (struct powertrio_client_d *) kmalloc( sizeof( struct powertrio_client_d ) );
        if ( (void*) Client == NULL ){
            panic("powertrio_initialize: [FAIL] Client\n");
        }
        Client->index = (int) i;
        Client->used  = TRUE;
        Client->magic = 1234;
        Client->window = NULL;
        Client->next = NULL;

        PowerTrio.list[i] = (unsigned long) Client;
    };

    PowerTrio.tail = 0;
    PowerTrio.head = 0;
    PowerTrio.selected = 0;

// done:
    PowerTrio.initialized = TRUE;
    return 0;
}


// Add a window in a given slot.
int powertrio_set_window ( int index, struct window_d *window )
{
    struct powertrio_client_d *c;


    if (PowerTrio.initialized != TRUE ){
        debug_print ("powertrio_set_window: Power Trio is not initialized\n");
        return -1;
    }

    if (index<0)   {  return -1;  }
    if (index >= 3){  return -1;  }

    if ( (void*) window == NULL ){  return -1;  }

    if (window->used != TRUE || window->magic != 1234 )
    {
        return -1;
    }

    // Get client for this index

    c = (struct powertrio_client_d *) PowerTrio.list[index];

    if ( (void*) c == NULL ){  return -1;  }

    if (c->used != TRUE || c->magic != 1234 )
    {
        return -1;
    }

    if (c->index != index){
        debug_print ("powertrio_set_window: [ERROR] c->index\n");
        return -1;
    }

    // Saving.
    c->window = (struct window_d *) window;
    
    // #todo
    // Select ?
    
    return 0;
}


// Service 302:
// Add a window in the next slot of a circular queue.
// Coloca a janela em um dos 3 clientes do Power Trio.
int powertrio_queue( struct window_d *window )
{

    if (PowerTrio.initialized != TRUE ){
        debug_print ("powertrio_queue: Power Trio is not initialized\n");
        return -1;
    }

    if ( (void*) window == NULL ){  return -1;  }
    
    if (window->used != TRUE || window->magic != 1234 )
    {
        return -1;
    }

    if (window->type != WT_OVERLAPPED ){
        debug_print ("powertrio_queue: invalid type \n");
        return -1;
    }

    // check
    if ( PowerTrio.tail <= 0 || PowerTrio.tail >= 3 )
    {
        PowerTrio.tail = 0;
    }

    // Coloca essa janela nesse dado slot.
    powertrio_set_window (
        PowerTrio.tail,     // Slot
        window );           // Window

    
    // Circula
    PowerTrio.tail++;
    
    if ( PowerTrio.tail >= 3 ){
        PowerTrio.tail = 0;
    }

    return 0;
}


// Service 303:
// Rearrange the windows in the screen
// and refresh the screen.
// Rearrange e atualize o power trio.
int powertrio_arrange_and_update(void)
{
    int i=0;
    struct powertrio_client_d *c;

    struct window_d *window;

    // Limiting the name size.
    char NameBuffer[32];

    // Screen info
    unsigned long DeviceWidth  = (unsigned long) screenGetWidth();
    unsigned long DeviceHeight = (unsigned long) screenGetHeight();

    // Client info
    unsigned long l = (unsigned long) 0;
    unsigned long t = (unsigned long) 0;
    unsigned long w = (unsigned long) 0;
    unsigned long h = (unsigned long) 0;


    int Selected = FALSE;
    int Update = FALSE;


    if (PowerTrio.initialized != TRUE ){
        debug_print ("powertrio_arrange_and_update: Power Trio is not initialized\n");
        return -1;
    }

    // main window
    // See: logon.c
    if ( (void*) gui == NULL ){
        panic ("powertrio_arrange_and_update: gui\n");
    }
    // Do now refresh now.
    if ( (void*) gui->main != NULL ){
        redraw_window ( gui->main, FALSE );
    }


    //
    // Loop
    //
    
    // Only three windows.
    
    for (i=0; i<3; i++){

    Selected = FALSE;
    Update = TRUE;
        
    // Get client for this index

    c = (struct powertrio_client_d *) PowerTrio.list[i];
    
    // N�o atualize esse cliente.
    if ( (void*) c == NULL ){  Update = FALSE;  }

    // N�o atualize esse cliente.
    if (c->used != TRUE || c->magic != 1234 )
    {  
        Update = FALSE;  
    }

    // Get the window for this client.
    window = (struct window_d *) c->window;

    // N�o atualize esse cliente.
    if( (void*) window == NULL){  Update = FALSE;  }

    // N�o atualize esse cliente.
    if (window->used != TRUE || window->magic != 1234 )
    { 
        Update = FALSE; 
    }

    //
    // Rearrange
    //

    // Use 'if' statement, not switch.

    if (i == 0){
        l=8; 
        t=8; 
        w = (DeviceWidth >> 1) -32; 
        h = DeviceHeight       -32;
        if ( PowerTrio.selected == 0){ Selected = TRUE; }
    }

    if (i == 1){
        l = DeviceWidth >> 1; 
        t = 8;
        w = (DeviceWidth  >> 1) -32;
        h = (DeviceHeight >> 1) -32;
        if ( PowerTrio.selected == 1){ Selected = TRUE; }
    }

    if (i == 2){
        l = DeviceWidth  >> 1;
        t = DeviceHeight >> 1;
        w = (DeviceWidth  >> 1) -32;
        h = (DeviceHeight >> 1) -32;
        if ( PowerTrio.selected == 2){ Selected = TRUE; }
    }

    //
    // Update
    //

    // S� atualizamos se n�o houve problemas.
    if ( Update == TRUE ){

        // Activate and set focus.
        // A janela m�o tem o foco, se alguem clicar em
        // alguma janela filha, ent�o o foco muda.
        if ( Selected == TRUE ) { 
            c->window->active = TRUE;  
            kgwmSetFocus(c->window);
        }

        // Deactivate and lose focus.
        if ( Selected == FALSE ){ 
            c->window->active = FALSE; 
            kgwmKillFocus(c->window);
        }

        // Replace, resize and redraw the window.
        replace_window( c->window, l, t );
        resize_window ( c->window, w, h );
        redraw_window ( c->window, TRUE );

        // Show the name, 
        // refresh the screen and 
        // send a message to the application to update the window.

        if ( Selected == TRUE )
        {
            // [debug]:: Name
            strncpy(NameBuffer, c->window->name, 32);
            NameBuffer[31]=0;
            draw_text(c->window,8,8,COLOR_YELLOW,NameBuffer);
            
            // Refresh the screen,
            // Remenber, we also apdated the root window.
            
            refresh_screen();
                
            // No message.
            if ( c->window->tid < 0 ){ return -1; }
            
            // Send message. (11216)
            // Envia uma mensagem para a thread pedindo
            // pra ela atualizar a janela principal.

            kgws_send_to_tid (
                (int)               c->window->tid,  // tid
                (struct window_d *) c->window,       // NULL
                (int)               11216,      // Message Code
                (unsigned long)     0,          //0 
                (unsigned long)     0 );        //0 
        }
    }
    };

// done:
    debug_print ("powertrio_arrange_and_update: done\n");
    return 0;    
}


// Service 304:
// Change the active window.
// Select the next client in the list.
// control + f4  
// control + f11 por enquanto.
int powertrio_next(void)
{
    if (PowerTrio.initialized != TRUE ){
        debug_print ("powertrio_next: Power Trio is not initialized\n");
        return -1;
    }

    // Circula
    PowerTrio.selected++;
    if ( PowerTrio.selected < 0 || PowerTrio.selected >= 3 )
    {
        PowerTrio.selected = 0;
    }

    // Rearrange and update.
    
    powertrio_arrange_and_update();
 
    return 0;
}

// Select a given client.
// This client will have the active window and the focus.
int powertrio_select_client(int index)
{
    if (PowerTrio.initialized != TRUE ){
        debug_print ("powertrio_select_client: Power Trio is not initialized\n");
        return -1;
    }    
    
    PowerTrio.selected = index;

    if ( PowerTrio.selected < 0 || PowerTrio.selected >= 3 )
    {
        PowerTrio.selected = 0;
    }

    powertrio_arrange_and_update();
    
    return 0;
}


//
// End.
//














