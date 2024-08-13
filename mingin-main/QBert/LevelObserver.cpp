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
#include "ServiceLocator.h"

namespace dae
{
	LevelObserver::LevelObserver(float transitionTime)
		: m_GameObject()
		, m_QBertComp()
		, m_Pyramid()
		, m_SpawnSlickSams()
		, m_SlickSamSpawnTimer()
		, m_SlickSamSpawnInterval()
		, m_SlickSamMoveInterval()
		, m_SpawnUggWrongways()
		, m_UggWrongwaySpawnTimer1()
		, m_UggWrongwaySpawnTimer2()
		, m_UggWrongSpawnInterval()
		, m_UggWrongMoveInterval()
		, m_LevelComplete()
		, m_AnimationTimer()
		, m_FullAnimationTime()
		, m_FlashingTimer()
		, m_FlashingColorTime()
		, m_PostAnimationTimer()
		, m_PostAnimationPause()
		, m_CurrentFlashingColor()
		, m_EverythingClear()
		, m_Level()
		, m_LevelTitleTimer(0.f)
		, m_LevelTitleScreenTime(transitionTime)
		, m_IsDeadQbert()
		, m_DeadQbertTimer()
		, m_DeadQbertMaxTime()
		, m_DeathEmptyScene()
		, m_DeathEmptySceneTimer()
		, m_DeathEmptySceneMaxTime()
	{
	}
	LevelObserver::LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid, int level,
		bool spawnSlickSams, bool spawnUggWrongways,
		float slickSamSpawnInterval, float slickSamMoveInterval,
		float uggWrongwaySpawnInterval, float uggWrongwayMoveInterval)
		: m_QBertComp(qBertComp)
		, m_GameObject(gameObject)
		, m_LevelComplete(false)
		, m_Pyramid(std::move(pyramid))
		, m_SpawnSlickSams(spawnSlickSams)
		, m_SlickSamSpawnTimer(0.f)
		, m_SlickSamSpawnInterval(slickSamSpawnInterval)
		, m_SlickSamMoveInterval(slickSamMoveInterval)
		, m_SlickSamCompVector()
		, m_SpawnUggWrongways(spawnUggWrongways)
		, m_UggWrongwaySpawnTimer1(0.f)
		, m_UggWrongwaySpawnTimer2(0.f)
		, m_UggWrongSpawnInterval(uggWrongwaySpawnInterval)
		, m_UggWrongMoveInterval(uggWrongwayMoveInterval)
		, m_UggWrongCompVector()
		, m_Level(level)
		, m_AnimationTimer(0.f)
		, m_FullAnimationTime(3.f)
		, m_FlashingTimer(.1f)
		, m_FlashingColorTime(.1f)
		, m_PostAnimationPause(2.f)
		, m_PostAnimationTimer(0.f)
		, m_EverythingClear(false)
		, m_CurrentFlashingColor(3)
		, m_LevelTitleScreenTime()
		, m_LevelTitleTimer()
		, m_IsDeadQbert(false)
		, m_DeadQbertTimer(0.f)
		, m_DeadQbertMaxTime(2.f)
		, m_DeathEmptyScene(false)
		, m_DeathEmptySceneTimer(0.f)
		, m_DeathEmptySceneMaxTime(1.f)
	{
		if (m_UggWrongSpawnInterval - 2.f > 0.f)
		{
			m_UggWrongwaySpawnTimer1 = m_UggWrongSpawnInterval - 2.f;
			m_UggWrongwaySpawnTimer2 = m_UggWrongSpawnInterval - 2.f;
		}
	}

	LevelObserver::~LevelObserver()
	{
		if (auto qBertComp = m_QBertComp.lock())
		{
			qBertComp->GetSubject()->RemoveObserver(this);
			qBertComp->Freeze(false);
		}
		for (auto& slickSam : m_SlickSamCompVector)
		{
			if (slickSam != nullptr)
				slickSam->GetSubject()->RemoveObserver(this);
		}

		for (auto& uggWrong : m_UggWrongCompVector)
		{
			if (uggWrong != nullptr)
				uggWrong->GetSubject()->RemoveObserver(this);
		}
	}

	void LevelObserver::Initialize()
	{
		if (auto qBertComp = m_QBertComp.lock())
		{
			qBertComp->GetSubject()->AddObserver(this);
			qBertComp->Freeze(false);
		}
	}

	void LevelObserver::Update()
	{
		if (m_LevelTitleScreenTime <= 0.f)
		{
			if (m_LevelComplete == false)
			{
				if (m_IsDeadQbert)
				{
					m_DeadQbertTimer += Timer::GetInstance().GetDeltaTime();
					if (m_DeadQbertTimer >= m_DeadQbertMaxTime)
					{
						if (auto qBertComp = m_QBertComp.lock())
						{
							qBertComp->HideTexture(true);
							qBertComp->HideCurses(true);
						}
						ClearLevel();
						m_DeadQbertTimer = 0.f;
						m_IsDeadQbert = false;
						m_DeathEmptyScene = true;
					}
				}
				else
				{
					if (m_DeathEmptyScene)
					{
						m_DeathEmptySceneTimer += Timer::GetInstance().GetDeltaTime();
						if (m_DeathEmptySceneTimer >= m_DeathEmptySceneMaxTime)
						{
							if (auto qBertComp = m_QBertComp.lock())
							{
								qBertComp->HideTexture(false);
								qBertComp->Freeze(false);
							}
							m_DeathEmptySceneTimer = 0.f;
							m_DeathEmptyScene = false;
						}
					}
					else
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

						if (m_SpawnUggWrongways)
						{
							m_UggWrongwaySpawnTimer1 += Timer::GetInstance().GetDeltaTime();
							m_UggWrongwaySpawnTimer2 += Timer::GetInstance().GetDeltaTime();

							if (m_UggWrongwaySpawnTimer1 >= m_UggWrongSpawnInterval)
							{
								bool isUgg = false;

								if ((rand() % 2) + 1 == 1)
									isUgg = true;

								AddUggWrongway(isUgg, true);
								m_UggWrongwaySpawnTimer1 -= m_UggWrongSpawnInterval;
							}
							if (m_UggWrongwaySpawnTimer2 >= m_UggWrongSpawnInterval + 1.f)
							{
								bool isUgg = false;

								if ((rand() % 2) + 1 == 1)
									isUgg = true;

								AddUggWrongway(isUgg, false);
								m_UggWrongwaySpawnTimer2 -= m_UggWrongSpawnInterval;
							}
						}
					}
				}
			}
			else
			{
				LevelWonAnimation();
			}
		}
		else
		{
			m_LevelTitleTimer += Timer::GetInstance().GetDeltaTime();

			if (m_LevelTitleTimer >= m_LevelTitleScreenTime)
				ChangeLevel();
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


	void LevelObserver::OnNotify(const Event& event)
	{
		switch (event)
		{
		case Event::QBertMoved:
			if (auto qBertComp = m_QBertComp.lock())
			{
				ServiceLocator::GetSoundSystem().PlaySound(std::rand() % 2 + 1, 50.f);
				const auto qBertIdx = qBertComp->GetPositionIndex();
				m_Pyramid->m_CubeGOVector[qBertIdx - 1]->GetComponent<Cube>()->TurnCube();

				if (CheckAllCubesTurned())
					WinLevel();

				KillCollidingSlickSam();

				if (CheckCollidingUggWrong())
				{
					ChangeFreezeEverything(true);
					m_IsDeadQbert = true;
					qBertComp->Die();

					qBertComp->HideCurses(false);
					ServiceLocator::GetSoundSystem().PlaySound(0, 50.f);
				}
			}
			break;

		case Event::SlickSamMoved:
			for (auto it = m_SlickSamCompVector.begin(); it != m_SlickSamCompVector.end();)
			{
				auto& slickSam = *it;
				const auto slickSamIdx = slickSam->GetPositionIndex();
				m_Pyramid->m_CubeGOVector[slickSamIdx - 1]->GetComponent<Cube>()->SlickSamTurnCube();
			}
			KillCollidingSlickSam();
			break;
		case Event::SlickSamFell:
			KillFallenSlickSam();
			break;
		case Event::UggWrongwayMoved:
			if (CheckCollidingUggWrong())
			{
				ChangeFreezeEverything(true);
				m_IsDeadQbert = true;
				m_QBertComp.lock()->Die();

				m_QBertComp.lock()->HideCurses(false);
				ServiceLocator::GetSoundSystem().PlaySound(0, 50.f);
			}
			break;
		case Event::UggWrongwayFell:
			KillFallenUggWrongway();
			ServiceLocator::GetSoundSystem().PlaySound(3, 50.f);
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

	void LevelObserver::AddUggWrongway(bool isUgg, bool isLeft)
	{
		auto newUggWrongwayGO = MakeUggWrongway(isUgg, isLeft, m_UggWrongMoveInterval);
		SceneManager::GetInstance().GetCurrentScene()->Add(newUggWrongwayGO);
		auto newUggWrongwayComp = newUggWrongwayGO->GetComponent<UggWrongWay>();

		if (newUggWrongwayComp != nullptr)
		{
			newUggWrongwayComp->GetSubject()->AddObserver(this);
			m_UggWrongCompVector.push_back(newUggWrongwayComp);
		}
		else
		{
			std::cout << "Uggwrongway creation failed\n";
		}

	}

	void LevelObserver::WinLevel()
	{
		if (!m_LevelComplete)
		{
			ServiceLocator::GetSoundSystem().PlaySound(5, 50.f);
			ChangeFreezeEverything(true);
			m_LevelComplete = true;
			m_Subject->Notify(Event::ColorChange);
		}
	}
	void LevelObserver::LevelWonAnimation()
	{
		m_AnimationTimer += Timer::GetInstance().GetDeltaTime();
		m_FlashingTimer += Timer::GetInstance().GetDeltaTime();

		if (m_AnimationTimer < m_FullAnimationTime)
		{
			if (m_FlashingTimer >= m_FlashingColorTime)
			{
				if (m_CurrentFlashingColor == 1)
				{
					for (const std::shared_ptr<GameObject>& cube : m_Pyramid->m_CubeGOVector)
						cube->GetComponent<Cube>()->TurnCubeSecondColor();

					m_CurrentFlashingColor = 2;
				}
				else if (m_CurrentFlashingColor == 2)
				{
					for (const std::shared_ptr<GameObject>& cube : m_Pyramid->m_CubeGOVector)
						cube->GetComponent<Cube>()->TurnCubeThirdColor();

					m_CurrentFlashingColor = 3;
				}
				else
				{
					for (const std::shared_ptr<GameObject>& cube : m_Pyramid->m_CubeGOVector)
						cube->GetComponent<Cube>()->TurnCubeFirstColor();

					m_CurrentFlashingColor = 1;
				}

				m_FlashingTimer = 0.f;
			}
		}
		else
		{
			if (m_EverythingClear == false)
			{
				m_EverythingClear = true;
				ClearLevel();
				m_QBertComp.lock()->HideTexture(true);

				for (const std::shared_ptr<GameObject>& cube : m_Pyramid->m_CubeGOVector)
				{
					if (m_Level == 2)
						cube->GetComponent<Cube>()->TurnCubeThirdColor();
					else
						cube->GetComponent<Cube>()->TurnCubeSecondColor();
				}
			}

			m_PostAnimationTimer += Timer::GetInstance().GetDeltaTime();
			if (m_PostAnimationTimer >= m_PostAnimationPause)
			{
				m_QBertComp.lock()->Freeze(false);
				ChangeLevel();
			}
		}
	}
	void LevelObserver::ChangeFreezeEverything(bool freeze)
	{
		if (auto qBertComp = m_QBertComp.lock())
		{
			qBertComp->Freeze(freeze);
		}
		for (auto i = 0; i < m_SlickSamCompVector.size(); i++)
			m_SlickSamCompVector[0]->Freeze(freeze);;

		for (auto i = 0; i < m_UggWrongCompVector.size(); i++)
			m_UggWrongCompVector[0]->Freeze(freeze);;

	}
	void LevelObserver::ChangeLevel() const
	{
		if (auto qBertComp = m_QBertComp.lock())
		{
			qBertComp->ResetPosition();
			qBertComp->Freeze(true);
		}
		auto& scene = SceneManager::GetInstance();
		scene.ChangeScene(scene.GetCurrentSceneIdx() + 1);
	}
	void LevelObserver::ClearLevel()
	{
		auto nrComponents = m_SlickSamCompVector.size();
		for (int i = 0; i < nrComponents; i++)
		{
			auto slickSam = m_SlickSamCompVector.operator[](0);
			m_SlickSamCompVector.erase(m_SlickSamCompVector.begin());
			slickSam->Die();
		}

		m_SlickSamSpawnTimer = 0.f;


		nrComponents = m_UggWrongCompVector.size();
		for (int i = 0; i < nrComponents; i++)
		{
			auto uggWrong = m_UggWrongCompVector.operator[](0);
			m_UggWrongCompVector.erase(m_UggWrongCompVector.begin());
			uggWrong->Die();
		}

		m_UggWrongwaySpawnTimer1 = 0.f;
		m_UggWrongwaySpawnTimer2 = -2.f;
	}
	bool LevelObserver::CheckCollidingUggWrong() const
	{
		for (int i = 0; i < m_UggWrongCompVector.size(); i++)
		{
			auto uggWrong = m_UggWrongCompVector.operator[](i);
			if (uggWrong->GetStartedLeft())
			{
				if (m_QBertComp.lock()->GetPositionIndex() == uggWrong->GetPositionIndex() + uggWrong->GetCurrentRow())
				{
					return true;
				}
			}
			else
			{
				if (m_QBertComp.lock()->GetPositionIndex() == uggWrong->GetPositionIndex() + uggWrong->GetCurrentRow() + 1)
				{
					return true;
				}
			}
		}

		return false;
	}
	void LevelObserver::KillCollidingSlickSam()
	{
		auto nrSlickSams = m_SlickSamCompVector.size();
		for (auto i = 0; i < nrSlickSams; i++)
		{
			if (m_QBertComp.lock()->GetPositionIndex() == m_SlickSamCompVector.operator[](i)->GetPositionIndex())
			{
				auto deadSlickSam = m_SlickSamCompVector.operator[](i);
				m_SlickSamCompVector.erase(std::find(m_SlickSamCompVector.begin(), m_SlickSamCompVector.end(), deadSlickSam));
				deadSlickSam->Die();
				i--;
				nrSlickSams--;
			}
		}
	}
	void LevelObserver::KillFallenSlickSam()
	{
		auto nrSlickSams = m_SlickSamCompVector.size();
		for (auto i = 0; i < nrSlickSams; i++)
		{
			auto slickSam = m_SlickSamCompVector.operator[](i);
			if (slickSam->GetIsAlive() == false)
			{
				m_SlickSamCompVector.erase(std::find(m_SlickSamCompVector.begin(), m_SlickSamCompVector.end(), slickSam));
				slickSam->Die();
				i--;
				nrSlickSams--;
			}
		}
	}
	void LevelObserver::KillFallenUggWrongway()
	{
		auto nrUggWrongs = m_UggWrongCompVector.size();
		for (auto i = 0; i < nrUggWrongs; i++)
		{
			auto uggWrong = m_UggWrongCompVector.operator[](i);
			if (uggWrong->GetIsAlive() == false)
			{
				m_UggWrongCompVector.erase(std::find(m_UggWrongCompVector.begin(), m_UggWrongCompVector.end(), uggWrong));
				uggWrong->Die();
				i--;
				nrUggWrongs--;
			}
		}
	}
}
