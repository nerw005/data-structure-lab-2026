#include "MainWindow.h"
#include "DataLoader.h"
#include <QHBoxLayout>
#include <QSplitter>
#include <QFrame>
#include <QString>

// 생성자: 데이터 초기화 후 UI 구성
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    DataLoader::init(graph);
    attackers = graph.getAttackers();
    targets   = graph.getTargets();
    allBuffs  = graph.getAllBuffs();

    setupUI();
    populateAttackers();
    populateTargets();
    if (!attackers.empty()) updateSkillCheckboxes(attackers[0]->index);
}

// 전체 UI 레이아웃 구성
void MainWindow::setupUI() {
    setWindowTitle("스킬 원콤 분석기");
    resize(880, 660);

    setStyleSheet(R"(
        QMainWindow, QWidget { background:#1e1e2e; color:#cdd6f4;
            font-family:'맑은 고딕'; font-size:13px; }
        QComboBox { background:#313244; border:1px solid #45475a;
            border-radius:4px; padding:4px 8px; }
        QComboBox::drop-down { border:none; }
        QComboBox QAbstractItemView { background:#313244;
            selection-background-color:#89b4fa; }
        QCheckBox { spacing:6px; }
        QCheckBox::indicator { width:16px; height:16px; border:1px solid #6c7086;
            border-radius:3px; background:#313244; }
        QCheckBox::indicator:checked { background:#89b4fa; border-color:#89b4fa; }
        QPushButton { background:#89b4fa; color:#1e1e2e; border:none;
            border-radius:5px; padding:8px 16px; font-weight:bold; font-size:14px; }
        QPushButton:hover  { background:#b4d0fb; }
        QPushButton:pressed{ background:#74a3e8; }
        QGroupBox { border:1px solid #45475a; border-radius:5px;
            margin-top:8px; padding-top:6px; }
        QGroupBox::title { subcontrol-origin:margin; left:8px; color:#a6adc8; }
        QScrollArea { border:none; }
    )");

    // ── 좌측 패널 (조작부) ──
    auto* leftScroll = new QScrollArea();
    auto* leftWidget = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(10);
    leftLayout->setContentsMargins(12, 12, 12, 12);

    auto* titleLabel = new QLabel(
        "<b style='font-size:16px; color:#cba6f7;'>⚔ 스킬 원콤 분석기</b>");
    leftLayout->addWidget(titleLabel);

    // 공격자 선택 드롭다운
    leftLayout->addWidget([]{ auto* l = new QLabel("공격자 선택");
        l->setStyleSheet("color:#a6adc8; font-size:11px;"); return l; }());
    cbAttacker = new QComboBox();
    leftLayout->addWidget(cbAttacker);

    // 스킬 선택 체크박스 그룹 (공격자 변경 시 동적으로 갱신)
    grpSkills       = new QGroupBox("스킬 선택");
    skillCheckLayout = new QVBoxLayout(grpSkills);
    skillCheckLayout->setSpacing(5);
    leftLayout->addWidget(grpSkills);

    // 공격 대상 선택 드롭다운
    leftLayout->addWidget([]{ auto* l = new QLabel("공격 대상 선택");
        l->setStyleSheet("color:#a6adc8; font-size:11px;"); return l; }());
    cbTarget = new QComboBox();
    leftLayout->addWidget(cbTarget);

    // 치명타 옵션
    chkCrit = new QCheckBox("치명타 포함 (머리 적중)");
    leftLayout->addWidget(chkCrit);

    // 강화제 선택 체크박스 그룹
    grpBuffers = new QGroupBox("강화제 선택 (중복 가능)");
    auto* bufLayout = new QVBoxLayout(grpBuffers);
    bufLayout->setSpacing(6);

    struct BInfo { QString label, desc; };
    std::vector<BInfo> bInfos = {
        {"아나",     "나노강화제  +50% 공격력"},
        {"메르시",   "카두세우스 지팡이  +25% 공격력"},
        {"바티스트", "증폭 매트릭스  +100% 공격력"},
        {"젠야타",   "부조화의 구슬  +30% 피격 데미지"}
    };
    for (int i = 0; i < static_cast<int>(bInfos.size()); ++i) {
        auto& bi = bInfos[i];

        // 체크박스 + 설명 라벨을 세로로 묶은 위젯
        auto* row    = new QWidget();
        auto* rowLay = new QVBoxLayout(row);
        rowLay->setContentsMargins(0, 2, 0, 2);
        rowLay->setSpacing(1);

        auto* chk  = new QCheckBox(bi.label);
        auto* desc = new QLabel("    " + bi.desc);
        desc->setStyleSheet("color:#6c7086; font-size:11px;");

        rowLay->addWidget(chk);
        rowLay->addWidget(desc);
        bufLayout->addWidget(row);
        buffChecks.push_back({chk, allBuffs[i]});
    }
    leftLayout->addWidget(grpBuffers);

    // 분석 실행 버튼
    btnAnalyze = new QPushButton("▶  원콤 여부 확인");
    leftLayout->addWidget(btnAnalyze);
    leftLayout->addStretch();

    leftScroll->setWidget(leftWidget);
    leftScroll->setWidgetResizable(true);
    leftScroll->setFixedWidth(320);
    leftScroll->setStyleSheet("QScrollArea{border:none;}");

    // ── 우측 패널 (결과 표시) ──
    auto* rightWidget = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(12, 12, 12, 12);
    rightLayout->setSpacing(14);

    lblResultTitle = new QLabel(
        "<b style='font-size:14px; color:#cba6f7;'>결과</b>");
    rightLayout->addWidget(lblResultTitle);

    // 원콤 가능/불가능 판정 라벨
    lblVerdict = new QLabel(
        "<center style='color:#6c7086; font-size:22px;'>"
        "조합을 선택하고<br>분석 버튼을 눌러주세요.</center>");
    lblVerdict->setWordWrap(true);
    lblVerdict->setAlignment(Qt::AlignCenter);
    lblVerdict->setMinimumHeight(80);
    lblVerdict->setStyleSheet(
        "background:#181825; border-radius:8px; padding:16px;");
    rightLayout->addWidget(lblVerdict);

    // 총 데미지 수치
    lblDamage = new QLabel();
    lblDamage->setWordWrap(true);
    lblDamage->setStyleSheet(
        "background:#181825; border-radius:6px; padding:12px;");
    lblDamage->hide();
    rightLayout->addWidget(lblDamage);

    // 사용 스킬 내역
    lblSkillDetail = new QLabel();
    lblSkillDetail->setWordWrap(true);
    lblSkillDetail->setStyleSheet(
        "background:#181825; border-radius:6px; padding:10px;");
    lblSkillDetail->hide();
    rightLayout->addWidget(lblSkillDetail);

    // 적용 버프 내역
    lblBuffDetail = new QLabel();
    lblBuffDetail->setWordWrap(true);
    lblBuffDetail->setStyleSheet(
        "background:#181825; border-radius:6px; padding:10px;");
    lblBuffDetail->hide();
    rightLayout->addWidget(lblBuffDetail);

    rightLayout->addStretch();

    // 좌우 분할
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(leftScroll);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(2);
    splitter->setStyleSheet("QSplitter::handle{background:#45475a;}");
    setCentralWidget(splitter);

    // 시그널-슬롯 연결
    connect(cbAttacker, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onAttackerChanged);
    connect(btnAnalyze, &QPushButton::clicked, this, &MainWindow::onAnalyze);
}

void MainWindow::populateAttackers() {
    for (auto* a : attackers)
        cbAttacker->addItem(QString::fromStdString(a->name));
}

void MainWindow::populateTargets() {
    for (auto* t : targets)
        cbTarget->addItem(
            QString::fromStdString(t->name) +
            QString("  (HP %1)").arg(t->hp));
}

// 공격자 변경 시 스킬 체크박스를 새로 생성
void MainWindow::updateSkillCheckboxes(int attackerIdx) {
    skillChecks.clear();

    // 기존 체크박스 제거
    QLayoutItem* item;
    while ((item = skillCheckLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // 새 체크박스 생성
    auto skills = graph.getSkills(attackerIdx);
    for (auto* s : skills) {
        QString dmgInfo;
        if (s->isInstakill) {
            dmgInfo = "즉사";
        } else if (s->initialDamage > 0) {
            dmgInfo = QString("초기 %1  +  초당 %2")
                          .arg(s->initialDamage).arg(s->perSecondDamage);
        } else {
            int base = s->baseDamage * s->projectileCount;
            int crit = s->critDamage * s->projectileCount;
            dmgInfo  = QString("기본 %1").arg(base);
            if (crit > 0) dmgInfo += QString("  /  치명타 %1").arg(crit);
            if (s->projectileCount > 1)
                dmgInfo += QString("  (×%1발)").arg(s->projectileCount);
            if (s->isPerSecond) dmgInfo += "  /1초";
        }

        // 스킬명 체크박스 + 데미지 설명 라벨
        auto* row    = new QWidget();
        auto* rowLay = new QVBoxLayout(row);
        rowLay->setContentsMargins(0, 2, 0, 2);
        rowLay->setSpacing(1);

        auto* chk  = new QCheckBox(QString::fromStdString(s->name));
        auto* desc = new QLabel("    " + dmgInfo);
        desc->setStyleSheet("color:#6c7086; font-size:11px;");

        rowLay->addWidget(chk);
        rowLay->addWidget(desc);
        skillCheckLayout->addWidget(row);
        skillChecks.push_back({chk, s});
    }

    // 결과 초기화
    lblVerdict->setText(
        "<center style='color:#6c7086; font-size:22px;'>"
        "조합을 선택하고<br>분석 버튼을 눌러주세요.</center>");
    lblVerdict->setStyleSheet(
        "background:#181825; border-radius:8px; padding:16px;");
    lblDamage->hide();
    lblSkillDetail->hide();
    lblBuffDetail->hide();
}

// 공격자 드롭다운 변경 시 스킬 목록 갱신
void MainWindow::onAttackerChanged(int index) {
    if (index < 0 || index >= static_cast<int>(attackers.size())) return;
    updateSkillCheckboxes(attackers[index]->index);
}

// 분석 버튼 클릭 시 선택된 스킬+버프 수집 후 결과 표시
void MainWindow::onAnalyze() {
    int aIdx = cbAttacker->currentIndex();
    int tIdx = cbTarget->currentIndex();
    if (aIdx < 0 || tIdx < 0) return;

    // 체크된 스킬 수집
    std::vector<SkillNode*> selectedSkills;
    for (auto& [chk, sk] : skillChecks)
        if (chk->isChecked()) selectedSkills.push_back(sk);

    if (selectedSkills.empty()) {
        lblVerdict->setStyleSheet(
            "background:#181825; border-radius:8px; padding:16px;");
        lblVerdict->setText(
            "<center style='color:#fab387; font-size:16px;'>"
            "스킬을 하나 이상 선택하세요.</center>");
        lblDamage->hide();
        lblSkillDetail->hide();
        lblBuffDetail->hide();
        return;
    }

    // 체크된 버프 수집
    std::vector<BuffNode*> selectedBuffs;
    for (auto& [chk, bf] : buffChecks)
        if (chk->isChecked()) selectedBuffs.push_back(bf);

    showResult(selectedSkills, selectedBuffs,
               chkCrit->isChecked(), targets[tIdx]->hp);
}

// 선택한 조합의 데미지를 계산하고 결과 라벨 업데이트
void MainWindow::showResult(const std::vector<SkillNode*>& skills,
                             const std::vector<BuffNode*>&  buffs,
                             bool useCrit, int targetHp) {
    // 데미지 계산
    bool  instakill = false;
    float total     = 0.0f;

    for (auto* s : skills) {
        float d = s->getDamage(useCrit);
        if (d >= 99999.0f) { instakill = true; break; }
        total += d;
    }

    // 버프 배율 곱계산 (공격자 측 / 피격자 측 분리)
    float aMult = 1.0f, tMult = 1.0f;
    if (!instakill) {
        for (auto* b : buffs) {
            if (b->side == BuffSide::ATTACKER_BOOST) aMult *= b->multiplier;
            else                                      tMult *= b->multiplier;
        }
        total *= aMult * tMult;
    }

    bool canOneShot = instakill || (total >= static_cast<float>(targetHp));

    // 판정 표시
    if (canOneShot) {
        lblVerdict->setStyleSheet(
            "background:#1a2e1a; border-radius:8px; padding:16px;"
            "border:2px solid #a6e3a1;");
        lblVerdict->setText(
            "<center style='color:#a6e3a1; font-size:26px; font-weight:bold;'>"
            "✔ 원콤 가능!</center>");
    } else {
        lblVerdict->setStyleSheet(
            "background:#2e1a1a; border-radius:8px; padding:16px;"
            "border:2px solid #f38ba8;");
        lblVerdict->setText(
            "<center style='color:#f38ba8; font-size:26px; font-weight:bold;'>"
            "✘ 원콤 불가능</center>");
    }

    // 데미지 수치
    QString dmgText;
    if (instakill) {
        dmgText = "<b style='color:#f38ba8;'>즉사</b>  (황야의 무법자)";
    } else {
        QString color = canOneShot ? "#a6e3a1" : "#f38ba8";
        dmgText = QString(
            "총 데미지: <b style='color:%1; font-size:20px;'>%2</b>  /  "
            "대상 HP: <b>%3</b>")
            .arg(color).arg(total, 0, 'f', 1).arg(targetHp);

        if (!canOneShot) {
            float gap = static_cast<float>(targetHp) - total;
            dmgText += QString("<br><small style='color:#6c7086;'>%1 부족</small>")
                           .arg(gap, 0, 'f', 1);
        }
    }
    lblDamage->setText(dmgText);
    lblDamage->show();

    // 스킬별 데미지 내역
    QString skillText = "<b style='color:#89b4fa;'>사용 스킬</b><br>";
    for (auto* s : skills) {
        float d   = s->getDamage(useCrit);
        QString row = "• " + QString::fromStdString(s->displayName());
        if (!instakill)
            row += QString("  → <font color='#cba6f7'>%1</font>").arg(d, 0, 'f', 0);
        skillText += row + "<br>";
    }
    lblSkillDetail->setText(skillText);
    lblSkillDetail->show();

    // 버프 내역
    if (!instakill) {
        QString buffText = "<b style='color:#fab387;'>적용 강화제</b><br>";
        if (buffs.empty()) {
            buffText += "<font color='#6c7086'>없음</font>";
        } else {
            for (auto* b : buffs)
                buffText += QString("• %1  (%2)<br>")
                    .arg(QString::fromStdString(b->bufferName))
                    .arg(QString::fromStdString(b->description));
            buffText += QString(
                "<small style='color:#6c7086;'>"
                "공격자 배율 ×%1  /  피격 배율 ×%2</small>")
                .arg(aMult, 0, 'f', 2).arg(tMult, 0, 'f', 2);
        }
        lblBuffDetail->setText(buffText);
        lblBuffDetail->show();
    } else {
        lblBuffDetail->hide();
    }
}
