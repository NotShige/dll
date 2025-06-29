#pragma once

#include <string>
#include <unordered_map>

#include "libhat/Access.hpp"
#include "HitResult.hpp"

#include <Utils/VersionUtils.hpp>
#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>
#include <SDK/Client/Util/mce.hpp>

#include "LevelData.hpp"

enum PlayerListPacketType : int8_t {
    AddPlayerEntry = 0x0, // The names are just add and removed but i dont want to cause issues
    RemovePlayerEntry = 0x1,
};

class PlayerListEntry {
public:
    uint64_t id; // This is the ActorUniqueID
    mcUUID UUID;
    std::string name, XUID, platformOnlineId;
    int buildPlatform;
    PlayerSkin playerSkin;
    bool isTeacher, isHost, isSubClient;

    PlayerListEntry &operator=(const PlayerListEntry &other) {
        return *this;
    }
};

class PlayerListEntry_1_21_90 {
public:
    uint64_t id;
    mcUUID UUID;
    std::string name, XUID, platformOnlineId;
    int buildPlatform;
    PlayerSkin_1_21_90 playerSkin;
    bool isTeacher, isHost, isSubClient;

    PlayerListEntry_1_21_90 &operator=(const PlayerListEntry_1_21_90 &other) {
        return *this;
    }
};


class Level {
public:
    std::unordered_map<mcUUID, PlayerListEntry> &getPlayerMap() {
        if (VersionUtils::checkAboveOrEqual(21, 90)) {
            // For 1.21.90+ versions, create a temporary map on each call to avoid memory issues
            static thread_local std::unordered_map<mcUUID, PlayerListEntry> tempMap;

            auto &newMap = *hat::member_at<std::unordered_map<mcUUID, PlayerListEntry_1_21_90> *>(this, GET_OFFSET("Level::getPlayerMap"));

            tempMap.clear();

            for (const auto &[uuid, entry_1_21_90]: newMap) {
                PlayerListEntry entry;
                entry.id = entry_1_21_90.id;
                entry.UUID = entry_1_21_90.UUID;
                entry.name = entry_1_21_90.name;
                entry.XUID = entry_1_21_90.XUID;
                entry.platformOnlineId = entry_1_21_90.platformOnlineId;
                entry.buildPlatform = entry_1_21_90.buildPlatform;
                entry.isTeacher = entry_1_21_90.isTeacher;
                entry.isHost = entry_1_21_90.isHost;
                entry.isSubClient = entry_1_21_90.isSubClient;

                if (entry_1_21_90.playerSkin.impl) {
                    const auto &sourceSkin = entry_1_21_90.playerSkin.impl->mObject;
                    entry.playerSkin.mId = sourceSkin.mId;
                    entry.playerSkin.mPlayFabId = sourceSkin.mPlayFabId;
                    entry.playerSkin.mFullId = sourceSkin.mFullId;
                    entry.playerSkin.mResourcePatch = sourceSkin.mResourcePatch;
                    entry.playerSkin.mDefaultGeometryName = sourceSkin.mDefaultGeometryName;

                    // Copy Image data manually to avoid assignment issues
                    entry.playerSkin.mSkinImage.imageFormat = sourceSkin.mSkinImage.imageFormat;
                    entry.playerSkin.mSkinImage.mWidth = sourceSkin.mSkinImage.mWidth;
                    entry.playerSkin.mSkinImage.mHeight = sourceSkin.mSkinImage.mHeight;
                    entry.playerSkin.mSkinImage.mDepth = sourceSkin.mSkinImage.mDepth;
                    entry.playerSkin.mSkinImage.mUsage = sourceSkin.mSkinImage.mUsage;
                    entry.playerSkin.mSkinImage.mImageBytes = Blob(sourceSkin.mSkinImage.mImageBytes);

                    entry.playerSkin.mCapeImage.imageFormat = sourceSkin.mCapeImage.imageFormat;
                    entry.playerSkin.mCapeImage.mWidth = sourceSkin.mCapeImage.mWidth;
                    entry.playerSkin.mCapeImage.mHeight = sourceSkin.mCapeImage.mHeight;
                    entry.playerSkin.mCapeImage.mDepth = sourceSkin.mCapeImage.mDepth;
                    entry.playerSkin.mCapeImage.mUsage = sourceSkin.mCapeImage.mUsage;
                    entry.playerSkin.mCapeImage.mImageBytes = Blob(sourceSkin.mCapeImage.mImageBytes);
                }

                tempMap.emplace(uuid, std::move(entry));
            }

            return tempMap;
        } else {
            if (VersionUtils::checkAboveOrEqual(21, 40)) {
                return *hat::member_at<std::unordered_map<mcUUID, PlayerListEntry> *>(this, GET_OFFSET("Level::getPlayerMap"));
            } else {
                return hat::member_at<std::unordered_map<mcUUID, PlayerListEntry> >(this, GET_OFFSET("Level::getPlayerMap"));
            }
        }
    }


    HitResult &getHitResult() {
        static int offset = GET_OFFSET("Level::hitResult");

        if (VersionUtils::checkAboveOrEqual(20, 60)) return *hat::member_at<std::shared_ptr<HitResult> >(this, offset);

        return hat::member_at<HitResult>(this, offset);
    }

    LevelData *getLevelData() { return hat::member_at<LevelData *>(this, GET_OFFSET("Level::LevelData")); };
    std::string getWorldFolderName() { return hat::member_at<std::string>(this, GET_OFFSET("Level::worldFolderName")); }

    std::vector<Actor *> getRuntimeActorList();
};