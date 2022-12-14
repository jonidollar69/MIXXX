#pragma once

#include "widget/woverview.h"

class WOverviewLMH : public WOverview {
    Q_OBJECT
  public:
    WOverviewLMH(
            const QString& group,
            PlayerManager* pPlayerManager,
            UserSettingsPointer pConfig,
            QWidget* parent = nullptr);

  private:
    bool drawNextPixmapPart() override;
};
