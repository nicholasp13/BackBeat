#include "bbpch.h"

#include "Player.h"
namespace BackBeat {

	Player::Player()
		: m_SelectedTrack(nullptr), m_PlayerProcessor(std::make_shared<PlayerProcessor>())
	{

	}

	Player::~Player()
	{
		m_PlayerProcessor->Off();
		delete m_SelectedTrack;
	}

	void Player::Start()
	{
		if (m_PlayerProcessor->IsOn())
			return;
		if (!m_SelectedTrack)
			return;
		if (m_SelectedTrack->IsDone())
			m_SelectedTrack->SetPosition(0);
		m_PlayerProcessor->PlayTrack(m_SelectedTrack);
		m_PlayerProcessor->On();
	}

	void Player::Stop()
	{
		m_PlayerProcessor->Off();
		if (m_SelectedTrack)
			m_SelectedTrack->SetPosition(0);
	}

	void Player::LoadTrack(std::string filePath)
	{
		m_PlayerProcessor->Off();
		delete m_SelectedTrack;
		m_SelectedTrack = TrackFactory::BuildTrack(filePath);
	}

	TimeMinSec Player::GetTime()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetTime();
		return TimeMinSec();
	}

	TimeMinSec Player::GetLength()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetLength();
		return TimeMinSec();
	}

	unsigned int Player::GetByteRate()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetProps().byteRate;
		return 1;
	}

	unsigned int Player::GetPosition()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetPosition();
		return 0;
	}

	unsigned int Player::GetSampleRate()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetProps().sampleRate;
		return 1;
	}

	unsigned int Player::GetSize()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetSize();
		return 0;
	}

	Track* Player::GetTrack()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack;
		return nullptr;
	}

	unsigned int Player::GetStartPosition()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetStart();
		return 0;
	}

	unsigned int Player::GetEndPosition()
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetEnd();
		return 0;
	}

	std::string Player::GetTrackName() 
	{
		if (m_SelectedTrack)
			return m_SelectedTrack->GetName();
		return "";
	}

	void Player::SetPosition(unsigned int pos)
	{
		if (m_SelectedTrack)
			m_SelectedTrack->SetPosition(pos);
	}

	void Player::SetStart(unsigned int start)
	{
		if (m_SelectedTrack)
			m_SelectedTrack->SetStart(start);
	}

	void Player::SetEnd(unsigned int end)
	{
		if (m_SelectedTrack)
			m_SelectedTrack->SetEnd(end);
	}

	void Player::SetVolume(float vol)
	{
		if (m_SelectedTrack)
			m_SelectedTrack->SetVolume(vol);
	}
}