/**
 * @file friend.h
 * @brief Friends (friend:*) service IPC wrapper.
 * @author yellows8
 * @copyright libnx Authors
 */
#pragma once
#include "../types.h"
#include "../kernel/event.h"
#include "../services/applet.h"
#include "../services/acc.h"
#include "../sf/service.h"

typedef enum {
    FriendServiceType_User        = 0, ///< Initializes friend:u
    FriendServiceType_V           = 1, ///< Initializes friend:v
    FriendServiceType_M           = 2, ///< Initializes friend:m
    FriendServiceType_S           = 3, ///< Initializes friend:s
    FriendServiceType_Application = 4  ///< Initializes friend:a
} FriendServiceType;

/// InAppScreenName
typedef struct {
    char name[0x40];                   ///< UTF-8 string, NUL-terminated.
    u64 languageCode;                  ///< LanguageCode, see set.h.
} FriendsInAppScreenName;

/// FriendInvitationGameModeDescription
typedef struct {
    u8 unk_x0[0xc00];                  ///< Unknown.
} FriendsFriendInvitationGameModeDescription;

/// FriendInvitationId
typedef struct {
    u64 id;                            ///< Id.
} FriendsFriendInvitationId;

/// FriendInvitationGroupId
typedef struct {
    u64 id;                            ///< Id.
} FriendsFriendInvitationGroupId;

/// FriendsUserSetting
typedef struct {
    AccountUid uid;                     ///< User ID
    u32 presence_permission;            ///< Presence permission
    u32 play_log_permission;            ///< Play log permission
    u64 friend_request_reception;       ///< Unknown
    char friend_code[0x20];             ///< Friend Code
    u64 friend_code_next_issuable_time; ///< Unknown
    char extra[0x7C8];                  ///< Extra
} FriendUserSetting;

/// Initialize friends
Result friendsInitialize(FriendServiceType service_type);

/// Exit friends
void friendsExit(void);

/// Gets the Service object for the friends service session.
Service * friendsGetServiceSession(void);

/// Gets the Service object for the actual IFriendsService service session.
Service* friendsGetServiceSession_IFriendsService(void);

/**
 * @brief Gets the \ref FriendUserSetting details
 * @param[in] uid \ref User AccountUid.
 * @param[out] user_setting \ref FriendUserSetting
 */
Result friendsGetUserSetting(AccountUid uid, FriendUserSetting * user_setting);

/**
 * @brief Gets an Event which is signaled when data is available with \ref friendsTryPopFriendInvitationNotificationInfo.
 * @note This is a wrapper for \ref appletGetFriendInvitationStorageChannelEvent, see that for the usage requirements.
 * @note The Event must be closed by the user once finished with it.
 * @param[out] out_event Output Event with autoclear=false.
 */
NX_INLINE Result friendsGetFriendInvitationNotificationEvent(Event *out_event) {
    return appletGetFriendInvitationStorageChannelEvent(out_event);
}

/**
 * @brief Uses \ref appletTryPopFromFriendInvitationStorageChannel then reads the data from there into the output params.
 * @note This is a wrapper for \ref appletTryPopFromFriendInvitationStorageChannel, see that for the usage requirements.
 * @param[out] uid \ref AccountUid. Optional, can be NULL.
 * @param[out] buffer Output buffer.
 * @param[out] size Output buffer size.
 * @param[out] out_size Size of the data which was written into the output buffer. Optional, can be NULL.
 */
Result friendsTryPopFriendInvitationNotificationInfo(AccountUid *uid, void* buffer, u64 size, u64 *out_size);
