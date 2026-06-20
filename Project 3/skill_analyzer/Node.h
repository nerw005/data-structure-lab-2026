#pragma once
#include <string>

enum class NodeType    { ATTACKER, SKILL, BUFFER, BUFF, TARGET };
enum class EdgeType    { HAS_SKILL, HAS_BUFF, CAN_BUFF, CAN_ATTACK };
enum class SkillCategory { BASIC, SKILL1, SKILL2, SKILL3, MELEE, ULTIMATE };
enum class BuffSide    { ATTACKER_BOOST, TARGET_DEBUFF };

struct GraphEdge {
    int      toIndex;
    EdgeType type;
};

// ─── Base ───────────────────────────────────────────────
class Node {
public:
    NodeType    type;
    std::string name;
    int         index = -1;
    virtual ~Node() = default;
};

// ─── Skill ──────────────────────────────────────────────
class SkillNode : public Node {
public:
    SkillCategory category        = SkillCategory::BASIC;
    int           baseDamage      = 0;
    int           critDamage      = 0;   // 0 = 치명타 없음
    int           projectileCount = 1;
    bool          isInstakill     = false;
    bool          isPerSecond     = false; // 표시용 (1초 기준 계산)
    int           initialDamage   = 0;    // 안란 궁극기 초기 폭발
    int           perSecondDamage = 0;    // 안란 궁극기 초당 데미지

    SkillNode() { type = NodeType::SKILL; }

    float getDamage(bool useCrit) const {
        if (isInstakill)      return 99999.0f;
        if (initialDamage > 0) return static_cast<float>(initialDamage + perSecondDamage);
        int dmg = (useCrit && critDamage > 0) ? critDamage : baseDamage;
        return static_cast<float>(dmg * projectileCount);
    }

    // 표시 이름 (발수, 초당, 즉사 표기 포함)
    std::string displayName() const {
        std::string s = name;
        if (isInstakill)       s += " [즉사]";
        else if (initialDamage > 0) s += " [궁극기]";
        else if (isPerSecond)  s += " [1초기준]";
        else if (projectileCount > 1) s += " (×" + std::to_string(projectileCount) + "발)";
        return s;
    }
};

// ─── Attacker ───────────────────────────────────────────
class AttackerNode : public Node {
public:
    AttackerNode() { type = NodeType::ATTACKER; }
};

// ─── Buffer ─────────────────────────────────────────────
class BufferNode : public Node {
public:
    BufferNode() { type = NodeType::BUFFER; }
};

// ─── Buff ───────────────────────────────────────────────
class BuffNode : public Node {
public:
    float       multiplier  = 1.0f;
    BuffSide    side        = BuffSide::ATTACKER_BOOST;
    std::string description;           // ex) "+50% 공격력"
    std::string bufferName;            // ex) "아나"

    BuffNode() { type = NodeType::BUFF; }
};

// ─── Target ─────────────────────────────────────────────
class TargetNode : public Node {
public:
    int hp = 0;
    TargetNode() { type = NodeType::TARGET; }
};
