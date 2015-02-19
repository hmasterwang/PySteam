/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
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


#ifndef ACHIEVEMENTS
#define ACHIEVEMENTS

#include <map>
#include <string>

#include <steam/steam_api.h>

extern char *ACHIEVEMENTS_DEFS[];
extern int ACHIEVEMENTS_COUNT;


class Achievements
{
public:
    bool setAchievement(const char* achID);

    static void init();
    static void finalize();
    static Achievements *instance();
    Achievements(char** achNames, int achNum);

private:
    static Achievements *_instance;


    int _appID;
    bool _statLoaded;
    char **_achName;
    int _achNum;

    std::map<std::string, bool> _achievements;

    STEAM_CALLBACK(Achievements, userStatArrived, UserStatsReceived_t, _callbackUserStatsReceived);
    STEAM_CALLBACK(Achievements, userStatsStored, UserStatsStored_t, _callbackUserStatsStored);
    STEAM_CALLBACK(Achievements, achievementStored, UserAchievementStored_t, _callbackAchievementStored);

//    void userStatArrived(UserStatsReceived_t *callback);
//    void userStatsStored(UserStatsStored_t *pCallback);
//    void achievementStored(UserAchievementStored_t *pCallback);
    bool requestStats();
};

#endif // ACHIEVEMENTS

