#include "LevelObserver.h"
#include <iostream>
#include "Cube.h"
#include "QBertCharacter.h"
#include "GameObject.h"
#include "Pyramid.h"
#include "Subject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Prototype.h"
#include "Timer.h"

namespace dae
{
    LevelObserver::LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid, bool spawnSlickSams, float slickSamSpawnInterval, float slickSamMoveInterval)
        : m_QBertComp(qBertComp)
        , m_GameObject(gameObject)
        , m_LevelComplete(false)
        , m_Pyramid(std::move(pyramid))
        , m_SpawnSlickSams(spawnSlickSams)
        , m_SlickSamSpawnTimer(0.f)
        , m_SlickSamSpawnInterval(slickSamSpawnInterval)
        , m_SlickSamMoveInterval(slickSamMoveInterval)
        , m_SlickSamCompVector()
    {
    }

    LevelObserver::~LevelObserver()
    {
        if (auto qBertComp = m_QBertComp.lock())
            qBertComp->GetSubject()->RemoveObserver(this);

        for (auto& slickSam : m_SlickSamCompVector)
        {
            if (slickSam != nullptr)
                slickSam->GetSubject()->RemoveObserver(this);
        }
    }

    void LevelObserver::Initialize()
    {
        if (auto qBertComp = m_QBertComp.lock())
            qBertComp->GetSubject()->AddObserver(this);
    }

    void LevelObserver::Update()
    {
        if (m_SpawnSlickSams)
        {
            m_SlickSamSpawnTimer += Timer::GetInstance().GetDeltaTime();

            if (m_SlickSamSpawnTimer >= m_SlickSamSpawnInterval)
            {
                bool isSlick = false;
                bool isLeft = false;

                if ((rand() % 2) + 1 == 1)
                    isSlick = true;

                if ((rand() % 2) + 1 == 1)
                    isLeft = true;

                AddSlickSam(isSlick, isLeft);
                m_SlickSamSpawnTimer -= m_SlickSamSpawnInterval;
            }
        }
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

    void LevelObserver::SetSlickSamVector(std::vector<std::shared_ptr<SlickSam>> slickSamCompVector)
    {
        for (auto& slickSam : m_SlickSamCompVector)
        {
            if (slickSam != nullptr)
                slickSam->GetSubject()->RemoveObserver(this);
        }

        m_SlickSamCompVector = std::move(slickSamCompVector);

        for (auto& slickSam : m_SlickSamCompVector)
        {
            if (slickSam != nullptr)
                slickSam->GetSubject()->AddObserver(this);
        }
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

                for (auto it = m_SlickSamCompVector.begin(); it != m_SlickSamCompVector.end();)
                {
                    if (qBertComp->GetPositionIndex() == (*it)->GetPositionIndex())
                    {
                        (*it)->Die();
                        it = m_SlickSamCompVector.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            break;

        case dae::Event::SlickSamMove:
            for (auto it = m_SlickSamCompVector.begin(); it != m_SlickSamCompVector.end();)
            {
                auto& slickSam = *it;
                const auto slickSamIdx = slickSam->GetPositionIndex();
                m_Pyramid->m_CubeGOVector[slickSamIdx - 1]->GetComponent<Cube>()->SlickSamTurnCube();

                if (auto qBertComp = m_QBertComp.lock())
                {
                    if (qBertComp->GetPositionIndex() == slickSamIdx)
                    {
                        slickSam->Die();
                        it = m_SlickSamCompVector.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            break;
        case dae::Event::SlickSamFell:
            for (int i = 0; i < m_SlickSamCompVector.size(); i++)
            {
                auto slickSam = m_SlickSamCompVector.operator[](i);
                if (slickSam->GetIsAlive() == false)
                {
                    m_SlickSamCompVector.erase(std::find(m_SlickSamCompVector.begin(), m_SlickSamCompVector.end(), slickSam));
                    slickSam->Die();
                }
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

    void LevelObserver::AddSlickSam(bool isSlick, bool isLeft)
    {
        auto newSlickSameGO = MakeSlickSam(isSlick, isLeft, m_SlickSamMoveInterval);
        SceneManager::GetInstance().GetCurrentScene()->Add(newSlickSameGO);
        auto newSlickSamComp = newSlickSameGO->GetComponent<SlickSam>();

        if (newSlickSamComp != nullptr)
        {
            newSlickSamComp->GetSubject()->AddObserver(this);
            m_SlickSamCompVector.push_back(newSlickSamComp);
        }
        else
        {
            std::cout << "Slick/Sam creation failed\n";
        }
    }

    void LevelObserver::WinLevel()
    {
        if (!m_LevelComplete)
        {
            m_LevelComplete = true;
            std::cout << "next level\n";
            m_Subject->Notify(Event::ColorChange);
            m_QBertComp.lock()->ResetPosition();
            auto& scene = SceneManager::GetInstance();
            scene.ChangeScene(scene.GetCurrentSceneIdx() + 1);
        }
    }
}
