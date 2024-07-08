#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

/***********global_variables****************/

int key[3][3]         ;
int key_trans[3][3]   ;
int key_inverse[3][3] ;
int determin_inver    ;

/***********determinant_Fun_def************/
int determinant ( int matrixs[3][3] );
/***********genration_matrix***************/
void genrate_key (void)     ;
/***********key_trans**********************/
void trans_key (void)   ;
/***********get_key_inverse****************/
void key_invers( void ) ;
/***********ENCRYPTION*********************/
string Encryption ( string & txt ) ;
/***********Decryption*********************/
string Decryption ( string & txt ) ;
/******************************************/
int main()
{
    genrate_key() ;
    trans_key  () ;
    key_invers () ;
    while (1)
    {
        char state ;
        cout << "Do you want to do \"E( Encrypt ) \" or \"D( Decrypt )\" \n" ;
        cin  >> state ;
        if ( (state != 'E' ) && (state != 'D') ) break ;
        string file_input, file_output ;
        cout << "Enter name of input file \n" ;
        cin  >> file_input ;
        ifstream input ( file_input ) ;
        string text ;
        char ch ;
        while ( input.get(ch) ) text += ch ;
        cout << "Enter name of output file \n" ;
        cin  >> file_output ;
        string result ;
        cout << endl ;
        switch (state)
        {
        case 'E' :
            result = Encryption( text ) ;
            break ;
        case 'D' :
            result = Decryption( text ) ;
            break ;
        }
        ofstream output ( file_output ) ;
        for( int i = 0 ; i < result.size() ; i++ )
            output.put( result[ i ] ) ;
    }
    return 0;
}
/***********determinant_Fun****************/
int determinant ( int matrixs[3][3] )
{
    int det ;
    int test = -1 ;
    det = (matrixs[0][0]  *( (matrixs[1][1] * matrixs[2][2]) - (matrixs[1][2] * matrixs[2][1]) ))
          -(matrixs[0][1] *( (matrixs[1][0] * matrixs[2][2]) - (matrixs[1][2] * matrixs[2][0]) ))
          +(matrixs[0][2] *( (matrixs[1][0] * matrixs[2][1]) - (matrixs[1][1] * matrixs[2][0]) )) ;

    det = det % 26 ;

    if ( det < 0 )
    {
        det = abs(det) % 26  ;
        det = 26 - det  ;
    }

    if ( (det % 2 == 0) || (det % 13 == 0) || (det == 0) )                  // to check the determinant
    {
        return -1 ;
    }
    for ( int i = 1 ; i <= 26  ; i++ )                       //to get determinant inverse
    {
        if ( (i * det) % 26 == 1 )
        {
            test = i ;
            break ;
        }
    }
    return test ;
}
/***********genration_matrix***************/
void genrate_key (void)
{
    determin_inver = -1 ;
    while ( determin_inver == -1  )
    {
        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
            {
                key[i][j] = rand() % 1000;
            }
        determin_inver = determinant(key) ;
    }
}
/***********key_trans**********************/
void trans_key (void)
{
    for ( int i = 0 ; i < 3 ; i++ )
    {
        for ( int j = 0 ; j < 3 ; j++ )
        {
            key_trans[i][j] = key[j][i] ;
        }
    }
}
/***********get_key_inverse****************/
void key_invers( void )
{
    int sign = 1 ;
    int counter  ;
    int minor[4] ;
    trans_key()  ;
    determin_inver = determinant(key) ;
    for ( int i = 0 ; i < 3 ; i++ )
        for ( int j = 0 ; j < 3 ; j++ )
        {
            counter = 0 ;
            for ( int row = 0 ; row < 3 ; row++ )
            {
                if ( row == i ) continue ;
                for ( int col = 0 ; col < 3 ; ++col )
                {
                    if ( col == j ) continue ;
                    minor[counter++] = key_trans[row][col] ;
                }
            }
            key_inverse[i][j] = ( minor[0] * minor[3] - minor[1] * minor[2] ) * sign  * determin_inver;
            sign *= -1 ;
            key_inverse[i][j] = (key_inverse[i][j]) % 26 ;
            if ( (key_inverse[i][j]) < 0 )
            {
                key_inverse[i][j] = abs(key_inverse[i][j]) % 26 ;
                key_inverse[i][j] = 26 - key_inverse[i][j] ;
            }
        }
}
/***********ENCRYPTION*********************/
string Encryption ( string & txt )
{
    string text = txt ;                                     //to avoid change in original text
    string output ;
    int arr[3]    ;
    int num       ;
    int len = txt.size() ;
    char counter  = ( len % 3 == 0  ) ? 0 : 3-(len%3) ;
    len += counter ;
    for ( int i = 0 ; i < 3 ; ++i ) text += "M" ;
    string change = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
    for ( int i = 0 ; i < len ; i+=3 )
    {
        for ( int j = 0 ; j < 3 ; ++j )
        {
            if (( (text[j+i] >= 'a') && (text[j+i] <= 'z') )
                ||((text[j+i]>= 'A') && (text[j+i] <= 'Z')))
            {
                if ( (text[j+i] >='A' )&& ((text[j+i]<= 'Z')))
                {
                    arr[j] = text[j+i] - 'A';
                }
                else
                {
                    arr[j] = text[j+i] - 'a';
                }
            }
            else if ( (text[j+i] - '0') == 0 )
            {
                arr[j] = -1 ;
            }
            else
            {
                ++len ;
                ++i   ;
                --j   ;
                text+="0" ;
            }
        }
        for ( int k = 0 ; k < 3 ; k++ )
        {
            if ((arr[0]) == -1 || ((arr[1]) == -1) || ((arr[2]) == -1) ) break ;
            num = arr[0] * key[k][0] + arr[1] * key[k][1] + arr[2] * key[k][2] ;
            num = num % 26 ;
            if ( num < 0 )
            {
                num = abs(num) % 26 ;
                num = 26 - num ;
            }
            output += change[num] ;
        }
    }
    return output ;
}
/***********Decryption*********************/
string Decryption ( string & txt )
{
    string output ;
    int arr[3] ;
    int num ;
    int len = txt.size() ;
    char counter  = ( len % 3 == 0  ) ? 0 : 3-(len%3) ;
    len += counter ;
    for ( int i = 0 ; i <= counter ; ++i ) txt += "0" ;
    string change = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
    for ( int i = 0 ; i < len ; i+=3 )
    {
        for ( int j = 0 ; j < 3 ; ++j )
        {
            if ( (txt[i + j] - '0') == 0  )
            {
                arr[j] = -1 ;
            }
            else
            {
                arr[j] = txt[i + j] - 'A';
            }
        }
        for ( int k = 0 ; k < 3 ; k++ )
        {
           if ((arr[0]) == -1 || ((arr[1]) == -1) || ((arr[2]) == -1) ) break ;
            num = arr[0] * key_inverse[k][0] + arr[1] * key_inverse[k][1] + arr[2] * key_inverse[k][2] ;
            num = num % 26 ;
            if ( num < 0 )
            {
                num = abs(num) % 26 ;
                num = 26 - num ;
            }
            output += change[num] ;
        }
    }
    return output ;
}
/******************************************/
