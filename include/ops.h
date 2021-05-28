#if !defined(OPS_H_)
#define OPS_H_

typedef enum
{
	/** 
	* \		operazioni che il server deve gestire
	*/

	OPEN_OP		=	0,	//	RICHIESTA DI APERTURA O DI CREAZIONE DI UN FILE.
	READ_OP		=	1,	//	RICHIESTA DI LEGGERE I DATI DI UN FILE.
	WRITE_OP	=	2,	//	RICHIESTA DI SCRITTURA IN UN FILE.
	APPEND_OP	=	3,	//	RICHIESTA DI CONCATENARE IL CONTENUTO DI UN FILE.
	CLOSE_OP	=	4,	//	RICHIESTA DI CHIUSURA DI UN FILE.
	REMOVE_OP	=	5,	//	RICHIESTA DI RIMOZIONE DI UN FILE.
	STATUS_OP	=	6,	//	CAMBIA LO STATO.

	/*
	* \aggiungere qui eltre operazioni che si vogliono implementare
	*/

	/** 
	* \		messaggi inviati dal server
	*/

	OP_OK		=	10,		//	OPERAZIONE ESEGUITA CON SUCCESSO
	OP_FOK		=	11,		//	OPERAZIONE ESEGUITA CON NON SUCCESSO
	OP_BLOCK	=	12,		//	FILE BLOCCATO
	OP_FFL_SUCH	=	13,		//	FILE RICHIESTO NON ESISTE
	OP_MSG_SIZE	=	14,		//	MESSAGGIO TROPPO LUNGO
		

	OP_END		=	20		//	RAGGIUNGO IL MASSIMO DI OPERATORI DISPONIBILI

}ops;

#endif // OPS_H_