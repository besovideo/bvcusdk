#ifndef __AAA_BASE__
#define __AAA_BASE__


#define AAA_ID_LEN     32  //ID�ų���
#define AAA_NAME_LEN   64  //���Ƴ���
#define AAA_PASSWD_LEN 64  //���볤��
#define AAA_PHONE_LEN  32  //�绰���볤��
#define AAA_EMAIL_LEN     128 //email length maximum
#define AAA_DESCRIPTION_LEN  1024   //������Ϣ����

#define AAA_PU_CHANNEL_MAX 63      //�豸ͨ���������ֵ
#define AAA_PTZ_LEVEL_MAX  63      //�����̨�����ȼ�ֵ

// ��Ȩ���жϣ�pΪȨ��ֵ���豸Ȩ��/ϵͳ����Ȩ)
#define AAAPermissionsHave(p)   (((unsigned char)(p) != 0) && ((unsigned char)(p) != 0xff))

// û��Ȩ���жϣ�pΪȨ��ֵ���豸Ȩ��/ϵͳ����Ȩ)
#define AAAPermissionsNone(p)   (((unsigned char)(p) == 0) || ((unsigned char)(p) == 0xff))

// ��a,b����Ȩ��ֵ�ϲ���c,��ֻ�����豸Ȩ��)��
// ʹ������磺���û����������ĳ���豸��ĳ��Ȩ������û����ͬһ�豸��ͬһȨ�޺ϲ�Ϊһ��ֵ����ֵΪ���û���Ը��豸����ʵȨ��
#define AAAPermissionsMix(a,b,c) {\
    if ((unsigned char)(a) == 0xff || (unsigned char)(b) == 0xff)\
        (c) = (unsigned char)(0x0ff);\
    else\
        (c) = (unsigned char)(a)>(unsigned char)(b)?(unsigned char)(a):(unsigned char)(b);\
}

#define AAA_GROUP_DEFAULT_GOD     "Admin"   //����,��ʼϵͳ������ID(ӵ�������豸�����ã����Ȩ)
#define AAA_USER_DEFAULT          "admin"   //����,��ʼϵͳ����ԱID(ӵ���û�(��)����Ȩ���豸����Ȩ���豸����Ȩ)

//  �豸Ȩ��ֵö��
enum
{
    AAA_PERMISSIONS_OFF = 0x00,  //û��Ȩ��
    AAA_PERMISSIONS_ON  = 0x01,  //Ȩ����Сֵ

    AAA_PERMISSIONS_MAX = 0xfe,  //Ȩ�����ֵ(�����Ȩ�����Ϳ������������ֵ���ƣ���PTZ)
    AAA_PERMISSIONS_BAN = 0xff,  //Ȩ�޽�ֹ(���κ�����ֵ���ʱ���������ִ�ֵ)
};

//  Ȩ��ֵ����
typedef unsigned char AAA_Permissions;

/* Ȩ��˵����
   ��1���豸��Ȩ��
     ��a��˵����
        �豸Ȩ�޷�Ϊ���û�����ԴȨ�� �� �û���ԴȨ�ޡ�Ȩ��ֵ�����AAA_PERMISSIONS_*,��غ�AAAPermissions*
        ���û�(��)û��ĳ����Դ��������Ը���Դ��ȫ��Ȩ��ֵΪAAA_PERMISSIONS_OFF
        �û����ĳ���豸��ĳ��Ȩ�޵�ʵ��ֵ = ���û���Ը��豸�ĸ�Ȩ��ֵ ��� ���û���������Ը��豸�ĸ�Ȩ��ֵ��
        �磺�û�U1�����û���G1����U1����豸P1��channel[0]ֵΪa��G1���P1��channel[0]ֵΪb,
            ��U1���P1��channel[0]��ʵ��Ȩ��ֵΪ AAAPermissionsMix(a,b,c)�е�c
     ��b�����������AAA_GROUP_DEFAULT_GOD��Ĭ�϶������豸��ȫ��Ȩ��ֵΪAAA_PERMISSIONS_ON
   ��2��ϵͳ����Ȩ�������û�(��)����Ȩ��,�豸����Ȩ���豸����Ȩ��
     ��a��˵����
        ϵͳ����Ȩ��ֵ�����AAA_PERMISSIONS_*,��غ�ΪAAAPermissionsHave/AAAPermissionsNone
        ӵ��ϵͳ����Ȩ���û�����ϵͳ����Ա��
     ��b�����������
        AAA_GROUP_DEFAULT_GOD���µ�ϵͳ����Ա���Թ���ȫ���û�(��),���ɿ��������
        AAA_GROUP_DEFAULT_GOD���²��ܴ������û��顣
*/

//  �豸Ȩ�޽ṹ�壬ֵ����� AAA_PERMISSIONS_*
typedef struct _aaa_pu_permissions_
{
    /*  �ṹ���С */
    int       iSize;

    /*  ����Ȩ��(�Ƿ�������ø��豸) */
    AAA_Permissions config;

    /*  �豸�ɼ�(�Ƿ���Կ������豸) */
    AAA_Permissions cansee;

    /*  ����,�ڴ���� */
    AAA_Permissions reserve[2];

    /*  ͨ��Ȩ�ޣ��±�Ϊͨ�������ţ� */
    AAA_Permissions channel[AAA_PU_CHANNEL_MAX+1];

}AAA_PUPermissions;

//  �û�Ȩ�޽ṹ�壬ֵ����� AAA_PERMISSIONS_*
typedef struct _aaa_user_permissions_
{
    /*  �ṹ���С */
    int             iSize;

    /*  �û������Ȩ(�Ƿ���Զ��û�����й���) */
    AAA_Permissions groupadmin;
    /*  �û�����Ȩ(�Ƿ���Զ��û����й���) */
    AAA_Permissions useradmin;
    /*  �豸����Ȩ(�Ƿ���Զ��豸���й���) */
    AAA_Permissions devadmin;
    /*  �豸����Ȩ(�Ƿ���Զ��豸���з���) */
    AAA_Permissions devassign;

    /*  ��̨�ȼ� */
    AAA_Permissions ptz;
    /*  �û����飩�ɼ���(AAA_PERMISSIONS_OFF�����ɼ���AAA_PERMISSIONS_ON��ͬ����AAA_PERMISSIONS_MAX���߼���) */
    AAA_Permissions cansee;
    /*  NRU/VTDU ����Ȩ */
    AAA_Permissions NRU;
    AAA_Permissions VTDU;

    /*  ������������Ȩ */
    AAA_Permissions alarmLinkage;
    /*  ����Χ������Ȩ */
    AAA_Permissions electricFence;
    /*  ����,�ڴ���� */
    AAA_Permissions reserve[2];
}AAA_UserPermissions;


#endif