// CPPTP2.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "CPPTP2.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Le texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // le nom de la classe de fenêtre principale
HANDLE FichierHandle;
WCHAR* FichierURL;
LARGE_INTEGER FichierLongueur;
CHAR* Texte = new CHAR[2]{ 0 };
CHAR* TexteATrouver = nullptr;
int LongueurTexte = 0;
int LongueurTexteATrouver = 0;
CHAR* Mot = new CHAR[0];
int IndexEmploye = 0;
int NombreEmploye;

HWND HandletxtFichier;
int editlength;

struct Employe
{
	char Nom[103]{ 0 };
	int Age;
	float Salaire;
};

// Pré-déclarations des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    File(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: placez ici le code.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPTP2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPTP2));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  BUT : inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPTP2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPPTP2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   BUT : enregistre le handle de l'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  BUT :  traite les messages pour la fenêtre principale.
//
//  WM_COMMAND - traite le menu de l'application
//  WM_PAINT - dessine la fenêtre principale
//  WM_DESTROY - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_Partie2:
			delete(FichierHandle);
			FichierHandle = CreateFile(
				_In_     L"db.bd",
				_In_     GENERIC_WRITE | GENERIC_READ,
				_In_     FILE_SHARE_READ,
				_In_opt_ NULL,
				_In_     OPEN_ALWAYS,
				_In_     FILE_ATTRIBUTE_NORMAL,
				_In_opt_ NULL
			);
			delete(HandletxtFichier);
			HandletxtFichier = GetDlgItem(hWnd, IDC_txtFicher);
			NombreEmploye = GetWindowTextLength(HandletxtFichier);
			NombreEmploye = ++NombreEmploye / sizeof(Employe);

			DialogBox(hInst, MAKEINTRESOURCE(IDD_File), hWnd, File);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ajoutez le code de dessin qui utilise hdc ici...


		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK File(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDC_btnEnregistrer:
		{
			

			Employe Emp;
			//buffers pour la lecture dans les editcontrols
			char* AgeEmploye = new char[4]{ 0 };
			char* SalaireEmploye = new char[10]{ 0 };

			
			//lecture des editcontrol
			GetDlgItemTextA(hDlg, IDC_txtNom, Emp.Nom, 103);
			GetDlgItemTextA(hDlg, IDC_txtAge, AgeEmploye, 4);
			GetDlgItemTextA(hDlg, IDC_txtSalaire, SalaireEmploye, 10);

			//conversion
			Emp.Age = atoi(AgeEmploye);
			Emp.Salaire = atof(SalaireEmploye);


			SetFilePointer(FichierHandle, IndexEmploye * sizeof(Emp), NULL, FILE_BEGIN);//Set le pointeur vers l'employe selectionne
			WriteFile(FichierHandle, &Emp, sizeof(Emp), NULL, NULL); //ecris les bytes de l'employe selectionne

			//vide le buffer
			FlushFileBuffers(
				_In_ FichierHandle
			);

			delete(SalaireEmploye);
			delete(AgeEmploye);
			//delete(&Emp);

			if (IndexEmploye == NombreEmploye)
				NombreEmploye++;
		}
			break;
		case IDC_btnPrecedent:

			if (IndexEmploye != 0)
			{
				IndexEmploye--;


				Employe Emps;
				SetFilePointer(FichierHandle, IndexEmploye * sizeof(Emps), NULL, FILE_BEGIN); //Set le pointeur vers l'employe selectionne
				ReadFile(FichierHandle, &Emps, sizeof(Emps), NULL, NULL); //lis les bytes de l'employe selection
				
																		  
				//affiche l'information pertinante dans les editcontrols
				SetDlgItemTextA(hDlg, IDC_txtNom, Emps.Nom);
				char buffer[10];
				sprintf_s(buffer, "%i", Emps.Age);
				SetDlgItemTextA(hDlg, IDC_txtAge, buffer);
				char buffer2[100];
				sprintf_s(buffer2, "%f", Emps.Salaire);
				SetDlgItemTextA(hDlg, IDC_txtSalaire, buffer2);
			}


			break;
		case IDC_btnSuivant:
			if (IndexEmploye < NombreEmploye)
			{
				IndexEmploye++;
				//efface les editcontrols si l'employé n'existe pas
				if (IndexEmploye == NombreEmploye)
				{
					SetDlgItemTextA(hDlg, IDC_txtNom, "");
					SetDlgItemTextA(hDlg, IDC_txtAge, "");
					SetDlgItemTextA(hDlg, IDC_txtSalaire, "");
				}
				else
				{
					Employe Emps;
					SetFilePointer(FichierHandle, IndexEmploye * sizeof(Emps), NULL, FILE_BEGIN); //Set le pointeur vers l'employe selectionne
					ReadFile(FichierHandle, &Emps, sizeof(Emps), NULL, NULL); //lis les bytes de l'employe selectionne
				    //affiche l'information pertinante dans les editcontrols
					SetDlgItemTextA(hDlg, IDC_txtNom, Emps.Nom);
					char buffer[10];
					sprintf_s(buffer, "%i", Emps.Age);
					SetDlgItemTextA(hDlg, IDC_txtAge, buffer);
					char buffer2[100];
					sprintf_s(buffer2, "%f", Emps.Salaire);
					SetDlgItemTextA(hDlg, IDC_txtSalaire, buffer2);
				}
			}
			else
			{
				//efface les editcontrols si l'employé n'existe pas
				if (IndexEmploye == NombreEmploye)
				{
					SetDlgItemTextA(hDlg, IDC_txtNom, "");
					SetDlgItemTextA(hDlg, IDC_txtAge, "");
					SetDlgItemTextA(hDlg, IDC_txtSalaire, "");
				}
			}
			
			break;
		case IDM_EXIT:
			delete(FichierHandle);
			DestroyWindow(hDlg);//Ferme tout
			break;
		default:
			return DefWindowProc(hDlg, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		// TODO: ajoutez le code de dessin qui utilise hdc ici...


		EndPaint(hDlg, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		int LongueurMot = 0; //variable comptant la longueur du mot en cours de verification
		int TempMotPosition = 0; //variable qui permet de sauvegarder l'index du dernier charactere d'un mot et ensuite de le parcourir
		int x = 0; //variable contenant l'espacement horrizontal des mots
		SIZE LargeurMot; //variable permettant de connaitre la largeur du mot en pixels
		int y = 5; //espacement vertical entre la premiere ligne et le haut du formulaire
		int i = 0; //index du charactere a lire
		int Match = 0; //variable comptant le nombre de characteres identiques dans 
		SetFilePointer(FichierHandle, 0, NULL, FILE_BEGIN); //Set le pointeur au bon endroit
		while (i < LongueurTexte && ReadFile(FichierHandle, Texte, 1, NULL, NULL)) //boucle qui lis les characteres dans le fichier jusqu'a la fin ou a une erreur
		{
			LongueurMot = 0; //set la longueur du mot actuel a 0 pour pouvoir le compter
			SetFilePointer(FichierHandle, i, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
			ReadFile(FichierHandle, Texte, 1, NULL, NULL); //lis un charactere
			while (i < LongueurTexte && *(Texte) != 015 && *(Texte) != ' ') //boucle permettant de distinguer les mots seprare par des espaces
			{
				i++;//incremente l'index du charactere a lire
				LongueurMot++; //compte le nombre de charactere dans le mot
		//?		SetFilePointer(FichierHandle, i, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
				ReadFile(FichierHandle, Texte, 1, NULL, NULL);//lis un charactere
			}
			TempMotPosition = LongueurMot; //copie la longueur du mots actuel dans une variable temporaire
			Match = LongueurMot; //set la variable Match a la longueur du mot pour pouvoir compter le nombre de characteres identiques
			delete(Mot); //efface la variable Mot pour lui donner une nouvelle valeur
			Mot = new CHAR[LongueurMot + 1]{ 0 }; //Donne a la variable Mot la bonne longueur
			TempMotPosition--; //Diminue la valeur de l'index temporaire
			if (LongueurMot == LongueurTexteATrouver) //verifie si la longeuure du Mot actuel est la meme que le mot est a trouver
			{
				while (TempMotPosition >= 0) //boucle qui parcourt la totalite du mot
				{
					SetFilePointer(FichierHandle, i - LongueurMot + TempMotPosition, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
					ReadFile(FichierHandle, Texte, 1, NULL, NULL);//lis un charactere
					if (*(Texte) == *(TexteATrouver + TempMotPosition)) //verifie si le charactere selectionne est le meme que le charatere du mot a trouver au meme index
						Match--; //decremente le compteur de charactere identique si le charactere est identique
					*(Mot + TempMotPosition) = *(Texte); //sauvegarde le charactere dans une autre variable
					TempMotPosition--; //decremente l'index temporaire de la position dans le mot
				}
				
			}
			else//si le mot actuel n'a pas la meme longueure que le mot a trouver
			{
				while (TempMotPosition >= 0) //boucle qui parcourt la totalite du mot
				{
					SetFilePointer(FichierHandle, i - LongueurMot + TempMotPosition, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
					ReadFile(FichierHandle, Texte, 1, NULL, NULL);//lis un charactere
					*(Mot + TempMotPosition) = *(Texte); //sauvegarde le charactere dans une autre variable
					TempMotPosition--;//decremente l'index temporaire de la position dans le mot
				}
			}
			if (Match == 0) //verifie si tout les characteres du mot actuel sont identique au mot a trouver
				SetTextColor(hdc, 0x0000FF); //change la couleur du texte pour le rouge
			else
				SetTextColor(hdc, 0x000000); //change la couleur du texte pour le noir
			GetTextExtentPoint32A(
				_In_  hdc,
				_In_  Mot,
				_In_  LongueurMot - 1,
				_Out_ &LargeurMot
			); //retourne la largeur, en pixels, du mot a afficher 


			TextOutA(hdc, x, y * 16, Mot, LongueurMot); //affiche le mot sur le formulaire
			x += (&LargeurMot)->cx + 12; //ajoute la largeur du mot actuel a la largeur des mots precedents
			SetFilePointer(FichierHandle, i, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
			ReadFile(FichierHandle, Texte, 1, NULL, NULL);//lis un charactere
			if (*(Texte) == 015) //verifie si le charactere correspond a un changement de ligne
			{
				i++; //incremente l'index du fichier pour sauter le charactere de la nouvelle ligne
				y++; //incremente la position verticale de la prochaine ligne de texte qui sera ecris
				x = 0; //remet la position horizontale de la prochaine ligne de texte a 0
			}
			i++; //incremente l'index du fichier pour lire le charactere du nouveau mot
			SetFilePointer(FichierHandle, i, NULL, FILE_BEGIN);//Set le pointeur au bon endroit
		}
		EndPaint(hDlg, &ps);
	}


    case WM_COMMAND:

        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

		if (LOWORD(wParam) == IDC_btnTrouve)
		{
			
			if (Texte == nullptr || *Texte == '\0')
			{
				MessageBoxA(hDlg, "Erreur", NULL, 0); //affiche un message si aucun document n'a ete selectionne
			}
			else
			{
				delete(TexteATrouver); //delete TexteATrouver pour lui assigner une nouvelle valeur
				HandletxtFichier = GetDlgItem(hDlg, IDC_txtMot); //cree un handle de type HWND pour obtenir la longueur totale du texte dans IDC_txtMot
				editlength = GetWindowTextLength(HandletxtFichier); //obtiens la longueur totale du texte

				TexteATrouver = new CHAR[editlength + 1]; //assigne une nouvelle valeur
				GetDlgItemTextA(hDlg, IDC_txtMot, TexteATrouver, editlength + 1); //recupere le texte

				LongueurTexteATrouver = editlength;	//?transfert la longueur du mot dans la bonne variable
				InvalidateRect(hDlg, NULL, true); //refresh le formulaire
			}
			InvalidateRect(hDlg, NULL, true);
		}


		if (LOWORD(wParam) == IDC_btnOuvrir)
		{
			HandletxtFichier = GetDlgItem(hDlg, IDC_txtFicher); // cree un handle de type HWND pour obtenir la longueur totale du texte dans IDC_txtFicher
			editlength = GetWindowTextLength(HandletxtFichier);//obtiens la longueur totale du texte
			delete(FichierURL);//delete FichierURL pour lui assigner une nouvelle valeur
			FichierURL = new WCHAR[editlength + 1]; //assigne une nouvelle valeur
			GetDlgItemText(hDlg, IDC_txtFicher, FichierURL, editlength + 1);//recupere le texte

			MessageBox(hDlg, FichierURL, L"edit text", 0); //affiche le chemin du fichier
			CloseHandle(FichierHandle); //s'assure que le fichier est ferme avant de le reouvrir
			FichierHandle = CreateFile(
				_In_     FichierURL,
				_In_     GENERIC_READ,
				_In_     FILE_SHARE_READ,
				_In_opt_ NULL,
				_In_     OPEN_EXISTING,
				_In_     FILE_ATTRIBUTE_NORMAL,
				_In_opt_ NULL
			); //ouvre le fichier

			if (INVALID_HANDLE_VALUE == FichierHandle)
			{
				MessageBox(hDlg, L"N'existe pas", FichierURL, 0); //indique si il y a eu une erreur lors de l'ouverture du fichier
			}
			else //si pas d'erreur
			{
				GetFileSizeEx(FichierHandle, &FichierLongueur); //obtient la longueur du fichier
				LongueurTexte = FichierLongueur.QuadPart; //garde en memoire la longueur du fichier
			}
			InvalidateRect(hDlg, NULL, true); //refresh le formulaire
			
		}
		break;
    }
    return (INT_PTR)FALSE;
}
