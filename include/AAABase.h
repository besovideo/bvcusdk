#ifndef __AAA_BASE__
#define __AAA_BASE__


#define AAA_ID_LEN     32  //ID号长度
#define AAA_NAME_LEN   64  //名称长度
#define AAA_PASSWD_LEN 64  //密码长度
#define AAA_PHONE_LEN  32  //电话号码长度
#define AAA_EMAIL_LEN     128 //email length maximum
#define AAA_DESCRIPTION_LEN  1024   //描述信息长度

#define AAA_PU_CHANNEL_MAX 63      //设备通道索引最大值
#define AAA_PTZ_LEVEL_MAX  63      //最高云台操作等级值

// 有权限判断，p为权限值（设备权限/系统管理权)
#define AAAPermissionsHave(p)   (((unsigned char)(p) != 0) && ((unsigned char)(p) != 0xff))

// 没有权限判断，p为权限值（设备权限/系统管理权)
#define AAAPermissionsNone(p)   (((unsigned char)(p) == 0) || ((unsigned char)(p) == 0xff))

// 将a,b两个权限值合并到c,（只能是设备权限)。
// 使用情况如：将用户所在组针对某个设备的某个权限与该用户针对同一设备的同一权限合并为一个值，该值为该用户针对该设备的真实权限
#define AAAPermissionsMix(a,b,c) {\
    if ((unsigned char)(a) == 0xff || (unsigned char)(b) == 0xff)\
        (c) = (unsigned char)(0x0ff);\
    else\
        (c) = (unsigned char)(a)>(unsigned char)(b)?(unsigned char)(a):(unsigned char)(b);\
}

#define AAA_GROUP_DEFAULT_GOD     "Admin"   //保留,初始系统管理组ID(拥有所有设备的配置，浏览权)
#define AAA_USER_DEFAULT          "admin"   //保留,初始系统管理员ID(拥有用户(组)管理权，设备管理权，设备分配权)

//  设备权限值枚举
enum
{
    AAA_PERMISSIONS_OFF = 0x00,  //没有权限
    AAA_PERMISSIONS_ON  = 0x01,  //权限最小值

    AAA_PERMISSIONS_MAX = 0xfe,  //权限最大值(具体的权限类型可能有其它最大值限制，如PTZ)
    AAA_PERMISSIONS_BAN = 0xff,  //权限禁止(与任何其它值结合时，都将保持此值)
};

//  权限值类型
typedef unsigned char AAA_Permissions;

/* 权限说明：
   （1）设备的权限
     （a）说明：
        设备权限分为：用户组资源权限 和 用户资源权限。权限值意义见AAA_PERMISSIONS_*,相关宏AAAPermissions*
        若用户(组)没有某个资源，则其针对该资源的全部权限值为AAA_PERMISSIONS_OFF
        用户针对某个设备的某项权限的实际值 = 该用户针对该设备的该权限值 结合 该用户所在组针对该设备的该权限值。
        如：用户U1属于用户组G1，若U1针对设备P1的channel[0]值为a，G1针对P1的channel[0]值为b,
            则U1针对P1的channel[0]的实际权限值为 AAAPermissionsMix(a,b,c)中的c
     （b）特殊情况：AAA_GROUP_DEFAULT_GOD组默认对所有设备的全部权限值为AAA_PERMISSIONS_ON
   （2）系统管理权（即：用户(组)管理权限,设备管理权，设备分配权）
     （a）说明：
        系统管理权限值意义见AAA_PERMISSIONS_*,相关宏为AAAPermissionsHave/AAAPermissionsNone
        拥有系统管理权的用户叫做系统管理员。
     （b）特殊情况：
        AAA_GROUP_DEFAULT_GOD组下的系统管理员可以管理全部用户(组),即可跨组操作。
        AAA_GROUP_DEFAULT_GOD组下不能创建子用户组。
*/

//  设备权限结构体，值意义见 AAA_PERMISSIONS_*
typedef struct _aaa_pu_permissions_
{
    /*  结构体大小 */
    int       iSize;

    /*  配置权限(是否可以配置该设备) */
    AAA_Permissions config;

    /*  设备可见(是否可以看到该设备) */
    AAA_Permissions cansee;

    /*  保留,内存对齐 */
    AAA_Permissions reserve[2];

    /*  通道权限（下标为通道索引号） */
    AAA_Permissions channel[AAA_PU_CHANNEL_MAX+1];

}AAA_PUPermissions;

//  用户权限结构体，值意义见 AAA_PERMISSIONS_*
typedef struct _aaa_user_permissions_
{
    /*  结构体大小 */
    int             iSize;

    /*  用户组管理权(是否可以对用户组进行管理) */
    AAA_Permissions groupadmin;
    /*  用户管理权(是否可以对用户进行管理) */
    AAA_Permissions useradmin;
    /*  设备管理权(是否可以对设备进行管理) */
    AAA_Permissions devadmin;
    /*  设备分配权(是否可以对设备进行分配) */
    AAA_Permissions devassign;

    /*  云台等级 */
    AAA_Permissions ptz;
    /*  用户（组）可见性(AAA_PERMISSIONS_OFF：不可见。AAA_PERMISSIONS_ON：同级。AAA_PERMISSIONS_MAX：高级别) */
    AAA_Permissions cansee;
    /*  NRU/VTDU 管理权 */
    AAA_Permissions NRU;
    AAA_Permissions VTDU;

    /*  报警联动管理权 */
    AAA_Permissions alarmLinkage;
    /*  电子围栏管理权 */
    AAA_Permissions electricFence;
    /*  保留,内存对齐 */
    AAA_Permissions reserve[2];
}AAA_UserPermissions;


#endif