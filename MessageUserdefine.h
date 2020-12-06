#pragma once

// #define RegisterURL  L"http://weijian-qadrm.mtq.tvm.cn/login"
// #define loginURl L"http://weijian-api-qadrm.mtq.tvm.cn/api/user/login"
// 
// #define ACCOUNT_LIST L"http://weijian-api-qadrm.mtq.tvm.cn/api/self-media/account-list"
// 
// #define ACCOUNT_LIST_By_user L"http://weijian-api-qadrm.mtq.tvm.cn/api/self-media/platform-list-by-user"
// #define ACCOUNT_INFO L"http://weijian-api-qadrm.mtq.tvm.cn/api/self-media/account-info"
// 
// #define Cookie_postURL L"http://weijian-api-qadrm.mtq.tvm.cn/api/self-media/set-cookie"

#define RegisterURL  L"https://api-web.weijian.video"
#define loginURl L"https://api-web.weijian.video/api/user/login"

#define ACCOUNT_LIST L"https://api-web.weijian.video/api/self-media/account-list"

#define ACCOUNT_LIST_By_user L"https://api-web.weijian.video/api/self-media/platform-list-by-user"
#define ACCOUNT_INFO L"https://api-web.weijian.video/api/self-media/account-info"

#define Cookie_postURL L"https://api-web.weijian.video/api/self-media/set-cookie"

/// api / self - media / check - version ? version = 1.01.0.02

#define checkUpdateURL L"https://api-web.weijian.video/api/self-media/check-version"


#define MSG_LOGIN_THIRDPART_HTTP  WM_USER + 1000

#define MSG_GetAccList_Http  WM_USER + 2000
#define MSG_GetAccList_Info_Http  WM_USER + 2001
#define MSG_GetAccList_by_user  WM_USER + 2002
#define MSG_UPdataHandImage WM_USER + 2003
#define MSG_SETCookie  WM_USER + 2004

#define MSG_GetAccList_search_Http  WM_USER + 2005

#define  MSG_CheckUpdate  WM_USER + 2006


#define browserAtt L"float=\"false\" group=\"group1\" align=\"left\" height=\"30\" maxwidth=\"174\" normalimage=\"file=\'tabitem.png\' source=\'0,0,174,32\'\" hotimage=\"file=\'tabitem.png\' source=\'174,0,348,32\'\" pushedimage=\"file=\'tabitem.png\' source=\'348,0,522,32\'\" selectedimage=\"file=\'tabitem.png\' source=\'348,0,522,32\'\" textalign=\"center\" iconImage=\"Time_Icon_Hot.png\" closeImage=\"close - nor.png\" closehotImage=\"close - hot.png\" closePadding=\"0, 10, 10, 0\" iconPadding=\"20, 10, 0, 0\" closesize=\"12, 12\" iconsize=\"13, 13\"";

#define tabid_mywejian_id L"10000"
//#define tabid_myMedia_id L"10001"
#define tabid_accmgr_id L"10002"
#define tabid_datastatu_id L"10003"


// #define tabid_mywejian_url L"http://weijian-qadrm.mtq.tvm.cn/"
// #define tabid_accmgr_url L"http://weijian-qadrm.mtq.tvm.cn/media_account"
// #define tabid_datastatu_url L"http://weijian-qadrm.mtq.tvm.cn/media_Statustics"

#define tabid_mywejian_url L"https://weijian.video/"
#define tabid_accmgr_url L"https://weijian.video/media_account"
#define tabid_datastatu_url L"https://weijian.video/media_all_statustics"


// 百家号 https://baijiahao.baidu.com/builder/author/register/index
// 企鹅号 https://om.qq.com/userAuth/index
// 头条号 https://sso.toutiao.com/login/?service=https://mp.toutiao.com/sso_confirm/?redirect_url=JTJG
// 趣头条 https://mp.qutoutiao.net/login
// 快手   https://www.kuaishou.com/account/login
// 大鱼号 https://mp.dayu.com
// 百花号 https://mp.cloud.huawei.com/mediaopenwebservice/portal/

#define baijiahao_URL L"https://baijiahao.baidu.com/builder/author/register/index"
#define qiehao_URL L"https://om.qq.com/userAuth/index"
#define toutiaohao_URL L"https://sso.toutiao.com/login/?service=https://mp.toutiao.com/sso_confirm/?redirect_url=JTJG"
#define qutoutiao_URL L"https://mp.qutoutiao.net/login"
#define kuaishou_URL L"https://www.kuaishou.com/account/login"
#define dayuhao_URL L"https://mp.dayu.com"
#define baihuahao_URL L"https://mp.cloud.huawei.com/mediaopenwebservice/portal/"

#define baijiahao_cookie_url L"https://baijiahao.baidu.com/?source=inner"
#define qiehao_cookie_url L"https://om.qq.com"
#define toutiaohao_cookie_url L"https://mp.toutiao.com/sso_confirm/?redirect_url=JTJG"
#define qutoutiao_cookie_url L"https://mp.qutoutiao.net/page/first-page"



// #define first L"document.querySelector('.index-btn-login').click();document.getElementById('#TANGRAM__PSP_4__footerULoginBtn').click();document.getElementById('#TANGRAM__PSP_4__userName').value = 13091748626;document.getElementById('#TANGRAM__PSP_4__password').value = 'ZJJS6688';document.getElementById('#TANGRAM__PSP_4__submit').click();"
// #define second L"document.getElementById('#TANGRAM__PSP_4__footerULoginBtn').click();"
// #define third L"document.getElementById('#TANGRAM__PSP_4__userName').value = 13091748626;"
// #define fore L"document.getElementById('#TANGRAM__PSP_4__password').value = 'ZJJS6688';"
// #define five L"document.getElementById('#TANGRAM__PSP_4__submit').click();"

#define baijiahao_JS L"document.querySelectorAll('.main-login')[0];loginBtn.click();document.getElementById('TANGRAM__PSP_4__footerULoginBtn');userLogin.click();"
#define qiehao_JS L"var loginMin = document.getElementsByClassName('other-type')[0]; loginMin.click();"
#define toutiaohao_JS L""
#define qutoutiao_JS L""
#define kuaishou_JS L""
//#define kuaishou_JS L"document.getElementsByClassName('svelte-1a50qtp')[0].classList.remove('is-active');document.getElementsByClassName('svelte-1a50qtp')[1].classList.add('is-active');var loginBtn = document.getElementsByClassName('change-login')[0];loginBtn.click();"
#define dayuhao_JS L""
#define baihuahao_JS L""
