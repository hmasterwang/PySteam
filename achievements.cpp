/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <alpha@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

/*
 * This file contains definition of the achievement. Modify it before you use it.
 */

#include "achievements.h"

Achievements *Achievements::_instance = 0;

Achievements *Achievements::instance()
{
    if (!_instance)
        init();
    return _instance;
}

void Achievements::init()
{
    if (!_instance)
        _instance = new Achievements(ACHIEVEMENTS_DEFS, ACHIEVEMENTS_COUNT);
}

void Achievements::finalize()
{
    if (_instance) {
        delete _instance;
        _instance = 0;
    }
}

Achievements::Achievements(char **achNames, int achNum) :
    _appID(0), _statLoaded(false),
    _achName(achNames), _achNum(achNum),
    _callbackUserStatsReceived(this, &Achievements::userStatArrived),
    _callbackUserStatsStored(this, &Achievements::userStatsStored),
    _callbackAchievementStored(this, &Achievements::achievementStored)
{
    _appID = SteamUtils()->GetAppID();
    if (!requestStats())
        fprintf(stderr, "Failed to request stat\n");
}

void Achievements::userStatArrived(UserStatsReceived_t *callback)
{
    if (_appID == callback->m_nGameID) {
        if (k_EResultOK == callback->m_eResult) {
            _statLoaded = true;
            _achievements.clear();

            for (int i = 0; i < _achNum; ++i) {
                bool achieved;
                SteamUserStats()->GetAchievement(_achName[i], &achieved);
                _achievements[std::string(_achName[i])] = achieved;
                fprintf(stderr, "ACH: %s, %s\n", _achName[i], achieved ? "UNLOCKED" : "LOCKED");
            }
        } else {
            fprintf(stderr, "RequestStats - failed, %d\n", callback->m_eResult);
        }
    }
}

void Achievements::userStatsStored(UserStatsStored_t *pCallback)
{
    if (_appID == pCallback->m_nGameID) {
        if (k_EResultOK == pCallback->m_eResult)
            fprintf(stderr, "Stored stats for Steam\n");
        else
            fprintf(stderr, "StatsStored - failed, %d\n", pCallback->m_eResult);
    }
}

void Achievements::achievementStored(UserAchievementStored_t *pCallback)
{
    if (_appID == pCallback->m_nGameID)
        fprintf(stderr, "Stored Achievement for Steam\n");
}

bool Achievements::setAchievement(const char* achID)
{
    if (_statLoaded) {
        SteamUserStats()->SetAchievement(achID);
        return SteamUserStats()->StoreStats();
    }
    return false;
}

bool Achievements::requestStats()
{
    if (!SteamUserStats() || !SteamUser())
        return false;

    if (!SteamUser()->BLoggedOn())
        return false;

    return SteamUserStats()->RequestCurrentStats();
}

