#include "LevelObserver.h"
#include <iostream>
#include "Cube.h"
#include "QBertCharacter.h"
#include "GameObject.h"
#include "Pyramid.h"
#include "Subject.h"
#include "SceneManager.h"

namespace dae
{
    LevelObserver::LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid)
        : m_QBertComp(qBertComp)
        , m_GameObject(gameObject)
        , m_LevelComplete(false)
    {
        m_Pyramid = std::move(pyramid);
    }

    LevelObserver::~LevelObserver()
    {
        if (auto qBertComp = m_QBertComp.lock())
            qBertComp->GetSubject()->RemoveObserver(this);
    }

    void LevelObserver::Initialize()
    {
        if (auto qBertComp = m_QBertComp.lock())
            qBertComp->GetSubject()->AddObserver(this);
    }

    void LevelObserver::SetQBert(const std::shared_ptr<QBertCharacter>& qBertComp)
    {
        if (auto oldQbertComp = m_QBertComp.lock())
            oldQbertComp->GetSubject()->RemoveObserver(this);

        m_QBertComp = qBertComp;

        if (auto newQbertComp = m_QBertComp.lock())
            newQbertComp->GetSubject()->AddObserver(this);
    }

    void LevelObserver::SetPyramid(std::unique_ptr<Pyramid> pyramid)
    {
        m_Pyramid = std::move(pyramid);
    }

    void LevelObserver::OnNotify(const dae::Event& event)
    {
        switch (event)
        {
        case dae::Event::QBertMove:
            if (auto qBertComp = m_QBertComp.lock())
            {
                const auto qBertIdx = qBertComp->GetPositionIndex();
                m_Pyramid->m_CubeGOVector[qBertIdx - 1]->GetComponent<Cube>()->TurnCube();
                if (CheckAllCubesTurned())
                    WinLevel();
            }
            break;
        }
    }

    bool LevelObserver::CheckAllCubesTurned() const
    {
        for (const std::shared_ptr<GameObject>& cube : m_Pyramid->m_CubeGOVector)
        {
            if (!cube->GetComponent<Cube>()->GetIsTurned())
                return false;
        }

        return true;
    }

    void LevelObserver::WinLevel()
    {
        if (!m_LevelComplete)
        {
            m_LevelComplete = true;
            std::cout << "next level\n";
            m_Subject->Notify(Event::ColorChange);
            auto& scene = SceneManager::GetInstance();
            scene.ChangeScene(scene.GetCurrentSceneIdx() + 1);
        }
    }
}
