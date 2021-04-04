#include "service_guard.h"
#include "services/friends.h"

static FriendServiceType g_friendsServiceType;

static Service g_friendSrv;
static Service g_friendsIFriendService;

NX_GENERATE_SERVICE_GUARD_PARAMS(friends, (FriendServiceType service_type), (service_type));

Result _friendsInitialize(FriendServiceType service_type) {
    Result rc = MAKERESULT(Module_Libnx, LibnxError_BadInput);

    g_friendsServiceType = service_type;

    switch (g_friendsServiceType) {
        case FriendServiceType_User:
            rc = smGetService(&g_friendSrv, "friend:u");
            break;
        case FriendServiceType_Administrator:
            rc = smGetService(&g_friendSrv, "friend:a");
            break;
        case FriendServiceType_Manager:
            rc = smGetService(&g_friendSrv, "friend:m");
            break;
        case FriendServiceType_Viewer:
            rc = smGetService(&g_friendSrv, "friend:v");
            break;
        case FriendServiceType_System:
            rc = smGetService(&g_friendSrv, "friend:s");
            break;
    }

    rc = serviceDispatch(&g_friendSrv, 0,
        .out_num_objects = 1,
        .out_objects = &g_friendsIFriendService,
    );

    return rc;
}

void _friendsCleanup(void) {
    serviceClose(&g_friendsIFriendService);
    serviceClose(&g_friendSrv);
}

Service* friendsGetServiceSession(void) {
    return &g_friendSrv;
}

Service* friendsGetServiceSession_IFriendsService(void)
{
    return &g_friendsIFriendService;
}

Result friendsGetUserSetting(AccountUid uid, FriendUserSetting *user_setting) {
    return serviceDispatchIn(&g_friendsIFriendService, 20800, uid,
        .buffer_attrs = { SfBufferAttr_HipcPointer | SfBufferAttr_Out | SfBufferAttr_FixedSize },
        .buffers = { { user_setting, sizeof(FriendUserSetting) } }
    );
}

Result friendsTryPopFriendInvitationNotificationInfo(AccountUid *uid, void* buffer, u64 size, u64 *out_size) {
    Result rc=0;
    AppletStorage storage;
    s64 storage_size=0;
    u64 data_size = size;
    AccountUid tmpuid={0};

    rc = appletTryPopFromFriendInvitationStorageChannel(&storage);
    if (R_SUCCEEDED(rc)) rc = appletStorageGetSize(&storage, &storage_size);
    if (R_SUCCEEDED(rc) && storage_size < sizeof(AccountUid)) rc = MAKERESULT(Module_Libnx, LibnxError_BadInput);
    if (R_SUCCEEDED(rc)) {
        storage_size-=sizeof(AccountUid);
        rc = appletStorageRead(&storage, 0, &tmpuid, sizeof(AccountUid));
        if (R_SUCCEEDED(rc)) {
            if (data_size > storage_size) data_size = storage_size;
            if (data_size) rc = appletStorageRead(&storage, sizeof(AccountUid), buffer, data_size);
            if (R_SUCCEEDED(rc)) {
                if (out_size) *out_size = data_size;
                if (uid) *uid = tmpuid;
            }
        }
    }

    appletStorageClose(&storage);
    return rc;
}
