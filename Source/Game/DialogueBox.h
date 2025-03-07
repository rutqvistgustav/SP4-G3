#pragma once
#include "GameObjectTag.h"
#include "nlohmann/json.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Interactable.h"

class Scene;
class Player;
class SpriteWrapper;
class TextWrapper;
class SpriteSheetAnimation;

class DialogueBox :
    public Interactable
{
public:

    DialogueBox(Scene* aScene, bool anIsStartDialog = false);

    void Init(std::string anID);

    virtual void Update(const float aDeltaTime, UpdateContext& anUpdateContext) override;
    virtual void Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext) override;

    virtual void OnInteract(Player* aPlayer) override;

    virtual void SetPosition(const CU::Vector2<float> aPosition) override;

    const std::string FindCharacter(const std::string someText);

protected:

    virtual void TriggerExit(GameObject* aGameObject) override;

private:

    void FillSlides(const std::string& aDialogText);

    inline bool ShouldShowDialog() const { return myCurrentPage >= 0; }

private:

    bool myIsStartDialog{};

    int myCurrentPage{ -1 };
    bool myHasRead{};

    std::unique_ptr<SpriteSheetAnimation> myIndicatorAnimator;
    std::shared_ptr<SpriteWrapper> myIndicator;
    std::shared_ptr<TextWrapper> myText;
    std::vector<std::string> myPages;

};
