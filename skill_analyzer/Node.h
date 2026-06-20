#pragma once
#include <string>

// 노드 종류
enum class NodeType    { ATTACKER, SKILL, BUFFER, BUFF, TARGET };
// 엣지 종류
enum class EdgeType    { HAS_SKILL, HAS_BUFF, CAN_BUFF, CAN_ATTACK };
// 스킬 카테고리
enum class SkillCategory { BASIC, SKILL1, SKILL2, SKILL3, MELEE, ULTIMATE };
// 버프 적용 대상 (공격자 or 피격자)
enum class BuffSide    { ATTACKER_BOOST, TARGET_DEBUFF };

// 인접 리스트에 저장되는 엣지
struct GraphEdge {
    int      toIndex;
    EdgeType type;
};

// 모든 노드의 기본 클래스
class Node {
public:
    NodeType    type;
    std::string name;
    int         index = -1;
    virtual ~Node() = default;
};

// 스킬 노드 - 데미지 정보 포함
class SkillNode : public Node {
public:
    SkillCategory category        = SkillCategory::BASIC;
    int           baseDamage      = 0;
    int           critDamage      = 0;    // 0이면 치명타 없음
    int           projectileCount = 1;    // 동시 발사 수
    bool          isInstakill     = false; // 즉사 스킬 여부
    bool          isPerSecond     = false; // 초당 데미지 여부 (표시용)
    int           initialDamage   = 0;    // 안란 궁극기 초기 폭발
    int           perSecondDamage = 0;    // 안란 궁극기 초당 데미지

    SkillNode() { type = NodeType::SKILL; }

    // 치명타 여부에 따른 총 데미지 반환
    float getDamage(bool useCrit) const {
        if (isInstakill)       return 99999.0f;
        if (initialDamage > 0) return static_cast<float>(initialDamage + perSecondDamage);
        int dmg = (useCrit && critDamage > 0) ? critDamage : baseDamage;
        return static_cast<float>(dmg * projectileCount);
    }

    // UI 표시용 이름 (특수 스킬은 태그 추가)
    std::string displayName() const {
        std::string s = name;
        if (isInstakill)            s += " [즉사]";
        else if (initialDamage > 0) s += " [궁극기]";
        else if (isPerSecond)       s += " [1초기준]";
        else if (projectileCount > 1) s += " (×" + std::to_string(projectileCount) + "발)";
        return s;
    }
};

// 공격자 캐릭터 노드
class AttackerNode : public Node {
public:
    AttackerNode() { type = NodeType::ATTACKER; }
};

// 강화제를 주는 캐릭터 노드
class BufferNode : public Node {
public:
    BufferNode() { type = NodeType::BUFFER; }
};

// 강화제 노드 - 배율 정보 포함
class BuffNode : public Node {
public:
    float       multiplier  = 1.0f;
    BuffSide    side        = BuffSide::ATTACKER_BOOST;
    std::string description;
    std::string bufferName;

    BuffNode() { type = NodeType::BUFF; }
};

// 공격 대상 노드 - 체력 포함
class TargetNode : public Node {
public:
    int hp = 0;
    TargetNode() { type = NodeType::TARGET; }
};
