#include "DataLoader.h"

// ─── 헬퍼 ────────────────────────────────────────────────
SkillNode* DataLoader::makeSkill(
    const std::string& name, SkillCategory cat,
    int base, int crit, int proj,
    bool instakill, bool perSecond,
    int initDmg, int perSecDmg)
{
    auto* s           = new SkillNode();
    s->name           = name;
    s->category       = cat;
    s->baseDamage     = base;
    s->critDamage     = crit;
    s->projectileCount = proj;
    s->isInstakill    = instakill;
    s->isPerSecond    = perSecond;
    s->initialDamage  = initDmg;
    s->perSecondDamage = perSecDmg;
    return s;
}

static int addAttacker(Graph& g, const std::string& name,
                        std::vector<SkillNode*> skills)
{
    auto* a   = new AttackerNode();
    a->name   = name;
    int  aIdx = g.addNode(a);
    for (auto* sk : skills) {
        int skIdx = g.addNode(sk);
        g.addEdge(aIdx, skIdx, EdgeType::HAS_SKILL);
    }
    return aIdx;
}

static int addBuffer(Graph& g, const std::string& bufferName,
                      const std::string& buffName,
                      float mult, BuffSide side,
                      const std::string& desc)
{
    auto* buf     = new BufferNode();
    buf->name     = bufferName;
    int  bufIdx   = g.addNode(buf);

    auto* bf      = new BuffNode();
    bf->name       = buffName;
    bf->multiplier = mult;
    bf->side       = side;
    bf->description = desc;
    bf->bufferName  = bufferName;
    int  bfIdx    = g.addNode(bf);

    g.addEdge(bufIdx, bfIdx, EdgeType::HAS_BUFF);
    return bfIdx;
}

// ─── 메인 초기화 ─────────────────────────────────────────
void DataLoader::init(Graph& graph) {

    // ====================================================
    // 공격자 캐릭터
    // ====================================================

    // 캐서디
    addAttacker(graph, "캐서디", {
        makeSkill("피스키퍼",        SkillCategory::BASIC,   70, 140),
        makeSkill("섬광탄",          SkillCategory::SKILL1,  75),
        makeSkill("피스키퍼 난사",   SkillCategory::SKILL2,  50,  0, 6),
        makeSkill("주먹",            SkillCategory::MELEE,   40),
        // 궁극기: 상대 체력만큼 데미지 → 즉사
        []() {
            auto* s = new SkillNode();
            s->name        = "황야의 무법자";
            s->category    = SkillCategory::ULTIMATE;
            s->isInstakill = true;
            return s;
        }()
    });

    // 벤처
    addAttacker(graph, "벤처", {
        makeSkill("스마트 굴착기", SkillCategory::BASIC,    75),
        makeSkill("드릴 돌진",    SkillCategory::SKILL1,   90),
        makeSkill("잠복",         SkillCategory::SKILL2,  110),
        makeSkill("갈아버리기",   SkillCategory::MELEE,    70),
        makeSkill("지각 충격",    SkillCategory::ULTIMATE, 130)
    });

    // 겐지
    addAttacker(graph, "겐지", {
        makeSkill("수리검",   SkillCategory::BASIC,    27,  0, 3),  // 최대 3발
        makeSkill("질풍참",   SkillCategory::SKILL1,   50),
        makeSkill("주먹",     SkillCategory::MELEE,    40),
        makeSkill("용검",     SkillCategory::ULTIMATE, 110)
    });

    // 안란
    {
        auto* ultAnlan = new SkillNode();
        ultAnlan->name          = "불사조 승천";
        ultAnlan->category      = SkillCategory::ULTIMATE;
        ultAnlan->initialDamage  = 165;   // 초기 폭발
        ultAnlan->perSecondDamage = 45;   // 초당 데미지 (1초 기준)

        addAttacker(graph, "안란", {
            makeSkill("주작의 부채",    SkillCategory::BASIC,   22, 44, 2),  // 2발
            makeSkill("불난 데 부채질", SkillCategory::SKILL1,   6, 12, 6),  // 6발
            makeSkill("맹염 질주",      SkillCategory::SKILL2,  30),         // 1히트
            makeSkill("춤추는 불꽃",    SkillCategory::SKILL3,  50),
            makeSkill("주먹",           SkillCategory::MELEE,   40),
            ultAnlan
        });
    }

    // 리퍼
    {
        // 죽음의 꽃: 72 × 5발/초 = 360 (1초 기준)
        auto* ultReaper = new SkillNode();
        ultReaper->name           = "죽음의 꽃";
        ultReaper->category       = SkillCategory::ULTIMATE;
        ultReaper->baseDamage     = 72;
        ultReaper->projectileCount = 5;
        ultReaper->isPerSecond    = true;

        addAttacker(graph, "리퍼", {
            makeSkill("헬파이어 샷건",   SkillCategory::BASIC,  115, 230),
            makeSkill("긴박한 방아쇠",   SkillCategory::SKILL1,  96, 276),
            makeSkill("주먹",            SkillCategory::MELEE,   40),
            ultReaper
        });
    }

    // 파라
    {
        // 포화: 25(기본)/35(치명) × 30발/초 (1초 기준)
        auto* ultPharah = new SkillNode();
        ultPharah->name           = "포화";
        ultPharah->category       = SkillCategory::ULTIMATE;
        ultPharah->baseDamage     = 25;
        ultPharah->critDamage     = 35;
        ultPharah->projectileCount = 30;
        ultPharah->isPerSecond    = true;

        addAttacker(graph, "파라", {
            makeSkill("로켓 런처", SkillCategory::BASIC,   25, 125),
            makeSkill("충격탄",   SkillCategory::SKILL1,  30),
            makeSkill("주먹",     SkillCategory::MELEE,   40),
            ultPharah
        });
    }

    // 프레야
    addAttacker(graph, "프레야", {
        makeSkill("속사 석궁",   SkillCategory::BASIC,    30,  60),
        makeSkill("정조준",      SkillCategory::SKILL1,   35,  80),
        makeSkill("주먹",        SkillCategory::MELEE,    40),
        makeSkill("올가미 사격", SkillCategory::ULTIMATE, 25, 100)
    });

    // 벤데타
    addAttacker(graph, "벤데타", {
        makeSkill("팔라틴 팽",    SkillCategory::BASIC,    45, 120),
        makeSkill("칼날 투영",    SkillCategory::SKILL1,   70),
        makeSkill("소용돌이 질주",SkillCategory::SKILL2,   35,  70),
        makeSkill("치솟는 베기",  SkillCategory::SKILL3,   50),
        makeSkill("주먹",         SkillCategory::MELEE,    40),
        makeSkill("갈라내는 칼날",SkillCategory::ULTIMATE,100, 200)
    });

    // ====================================================
    // 강화제를 주는 캐릭터 (버퍼)
    // ====================================================
    addBuffer(graph, "아나",    "나노강화제",    1.5f,  BuffSide::ATTACKER_BOOST, "+50% 공격력");
    addBuffer(graph, "메르시",  "카두세우스 지팡이", 1.25f, BuffSide::ATTACKER_BOOST, "+25% 공격력");
    addBuffer(graph, "젠야타",  "부조화의 구슬", 1.3f,  BuffSide::TARGET_DEBUFF,   "+30% 피격 데미지");
    addBuffer(graph, "바티스트","증폭 매트릭스", 2.0f,  BuffSide::ATTACKER_BOOST, "+100% 공격력");

    // ====================================================
    // 공격을 받는 캐릭터 (타겟)
    // ====================================================
    auto addTarget = [&](const std::string& name, int hp) {
        auto* t = new TargetNode();
        t->name = name;
        t->hp   = hp;
        graph.addNode(t);
    };

    addTarget("트레이서",  175);
    addTarget("아나",      250);
    addTarget("키리코",    225);
    addTarget("시메트라",  275);
    addTarget("토르비욘",  300);
    addTarget("메르시",    225);
}
