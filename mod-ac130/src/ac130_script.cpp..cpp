#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Creature.h"
#include "ObjectAccessor.h"

// Global storage for config values
struct AC130Config
{
    bool enable;
    uint32 interval;
    uint32 cityProb;
    uint32 playerProb;
    float altitude;
    uint32 despawnTime;
    float bombRadius;
    float flySpeed;
    bool instantKill;
} acConfig;

enum AC130Entries
{
    NPC_ALLIANCE_AC130 = 34051,
    NPC_HORDE_AC130    = 34036,
    SPELL_BOMB_VISUAL  = 52438 
};

std::vector<uint32> AllyCities = { 1519, 1537, 1657, 3557 };
std::vector<uint32> HrdCities  = { 1637, 1497, 1638, 3487 };
std::vector<uint32> SafeZones  = { 10, 12, 14, 17, 33, 40, 405 };

class AC130WorldScript : public WorldScript
{
public:
    AC130WorldScript() : WorldScript("AC130WorldScript") {}

    uint32 _timer;

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        acConfig.enable      = sConfigMgr->GetOption<bool>("AC130.Enable", true);
        acConfig.interval    = sConfigMgr->GetOption<uint32>("AC130.Interval", 60) * 60000;
        acConfig.cityProb    = sConfigMgr->GetOption<uint32>("AC130.CityProbability", 5);
        acConfig.playerProb  = sConfigMgr->GetOption<uint32>("AC130.PlayerProbability", 30);
        acConfig.altitude    = sConfigMgr->GetOption<float>("AC130.Altitude", 80.0f);
        acConfig.despawnTime = sConfigMgr->GetOption<uint32>("AC130.DespawnTime", 600) * 1000;
        acConfig.bombRadius  = sConfigMgr->GetOption<float>("AC130.BombRadius", 30.0f);
        acConfig.flySpeed    = sConfigMgr->GetOption<float>("AC130.FlySpeed", 0.3f);
        acConfig.instantKill = sConfigMgr->GetOption<bool>("AC130.InstantKill", true);

        _timer = acConfig.interval;
    }

    void OnUpdate(uint32 diff) override
    {
        if (!acConfig.enable) return;

        if (_timer <= diff)
        {
            TriggerEvent();
            _timer = acConfig.interval;
        }
        else
            _timer -= diff;
    }

    void TriggerEvent()
    {
        uint32 zoneA = PickZone(true, 0);
        uint32 zoneH = PickZone(false, zoneA);

        SpawnFleet(zoneA, true);
        SpawnFleet(zoneH, false);
    }

    uint32 PickZone(bool isAlliance, uint32 exclude)
    {
        if (urand(0, 100) < acConfig.cityProb)
            return isAlliance ? AllyCities[urand(0, AllyCities.size() - 1)] : HrdCities[urand(0, HrdCities.size() - 1)];

        if (urand(0, 100) < acConfig.playerProb)
        {
            SessionMap const& sm = sWorld->GetAllSessions();
            if (!sm.empty()) {
                auto it = sm.begin(); std::advance(it, urand(0, sm.size() - 1));
                if (Player* p = it->second->GetPlayer())
                    if (p->GetZoneId() != exclude) return p->GetZoneId();
            }
        }
        return SafeZones[urand(0, SafeZones.size() - 1)];
    }

    void SpawnFleet(uint32 zoneId, bool isAlliance)
    {
        Player* anchor = nullptr;
        SessionMap const& sm = sWorld->GetAllSessions();
        for (auto const& it : sm) {
            if (it.second->GetPlayer() && it.second->GetPlayer()->GetZoneId() == zoneId) {
                anchor = it.second->GetPlayer(); break;
            }
        }

        if (!anchor) return;

        float x = anchor->GetPositionX();
        float y = anchor->GetPositionY();
        float z = anchor->GetPositionZ() + acConfig.altitude;
        uint32 entry = isAlliance ? NPC_ALLIANCE_AC130 : NPC_HORDE_AC130;

        for (int i = 0; i < 3; ++i)
        {
            if (Creature* zp = anchor->SummonCreature(entry, x + (i * 40), y + (i * 40), z, 0, TEMPSUMMON_TIMED_DESPAWN, acConfig.despawnTime))
            {
                zp->SetCanFly(true);
                zp->SetDisableGravity(true);
                zp->SetSpeed(MOVE_FLIGHT, acConfig.flySpeed);
                zp->GetMotionMaster()->MovePoint(1, x + 1500, y + 1500, z);
            }
        }

        std::string msg = isAlliance ? "|cff00ccff[AC130]|r Alliance bombing run started in this zone!" : "|cffff3300[AC130]|r Horde bombing run started in this zone!";
        ChatHandler(anchor->GetSession()).PSendSysMessage(msg.c_str());
    }
};

struct npc_ac130_zeppelin : public ScriptedAI
{
    npc_ac130_zeppelin(Creature* c) : ScriptedAI(c) {}
    uint32 bombTimer;

    void Reset() override {
        bombTimer = 2000;
        me->SetFaction(35); // Hostile to all
    }

    void UpdateAI(uint32 diff) override {
        if (bombTimer <= diff) {
            me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - acConfig.altitude, SPELL_BOMB_VISUAL, true);
            
            std::list<Unit*> targets;
            GetUnitListWithEntryInGrid(targets, me, 0, acConfig.bombRadius);
            for (Unit* target : targets) {
                if (target->IsAlive() && target->GetGUID() != me->GetGUID()) {
                    if (acConfig.instantKill)
                        me->DealDamage(target, target->GetHealth());
                    else
                        me->DealDamage(target, target->GetMaxHealth() * 0.5f);
                }
            }
            bombTimer = urand(1500, 3000);
        }
        else bombTimer -= diff;
    }
};

void Addmod_ac130Scripts()
{
    new AC130WorldScript();
    RegisterCreatureAI(npc_ac130_zeppelin);
}