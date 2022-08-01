import React, { useEffect, useState } from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography, Box} from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate, useParams} from 'react-router-dom';
import YavalathButton, {YavalathButtonFixed} from '../components/YavalathButton';
import ProfileWidget, {ProfileCard} from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import ButtonGroup from '@mui/material/ButtonGroup';
import ReusableToggleButtonGroup from '../components/ReusableToggleButtonGroup';
import ReusableToggleButton from '../components/ReusableToggleButton';
import ToggleButton from '@mui/material/ToggleButton';
import {Card2, Card3, statCard, MatchCard} from '../components/ReusableCard';
import CustomizedInputBase from '../components/SearchBar';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import StatTab from '../components/Tab';
import Modal from '@mui/material/Modal';
import ReactEcharts from 'echarts-for-react';




type Props = {}

type BoxContainerProps = {
  minHeight?: number;
}

const Container  = styled.div`
  min-height: 70vh;

  display: flex;
  flex-direction: Column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

const Container1  = styled.div`
  min-height: 450px;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;

  grid-gap: 80px;
`
const StatContainer  = styled.div`
  min-height: 450px;

  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 0px;
`

const TextContainer = styled.div `
  display: flex;
  flex-direction: column;

  align-items: center;
  grid-gap: 10px;
`


const Match = styled.div`
  width: 80vw;
  height: 150px;
  background: var(--accent-dark);
  cursor: pointer;

  display:flex;
  justify-content: center;
  align-items: center;
`


export type profileDataType = {
  uid: number;
  username: string;
  email: string;
  ranked: {
    CLASSIC: statsDataType;
    POTHOLES: statsDataType;
    TRIPLES: statsDataType;
  };
  casual: {
    CLASSIC: statsDataType;
    POTHOLES: statsDataType;
    TRIPLES: statsDataType;
  }
  "match_history_filtered": {
    "CLASSIC": replayType[]|null;
    "POTHOLES": replayType[]|null;
    "TRIPLES": replayType[]|null;
  }
}


type statsDataType = {
  draws: number;
  elo?: number;
  losses: number;
  wins: number;
}

const DefaultProfileData = {
  "email": "",
  "username": "",
  "uid": 0,
  "ranked": {
    "CLASSIC": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
      "elo": 1000,
    },
    "POTHOLES": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
      "elo": 1000
    },
    "TRIPLES": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
      "elo": 1000
    }
  },
  "casual": {
    "CLASSIC": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
    },
    "POTHOLES": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
    },
    "TRIPLES": {
      "draws": 0,
      "losses": 0,
      "wins": 0,
    }
  },
  "match_history_filtered": {
    "CLASSIC": null,
    "POTHOLES": null,
    "TRIPLES": null
  }
}


type replayType = {
  link: string;
  players: {
    username: string;
    elo_start: number;
  }[]
}


const tabMap = [
  'CLASSIC',
  'TRIPLES',
  'POTHOLES'
]

export type eloDataType = {
  CLASSIC: Array<number>;
  POTHOLES: Array<number>;
  TRIPLES: Array<number>;
}

const DefaultEloData = {
  "CLASSIC": [],
  "POTHOLES": [],
  "TRIPLES": []
}

const modalStyle = {
  position: 'absolute' as 'absolute',
  top: '50%',
  left: '50%',
  transform: 'translate(-50%, -50%)',
  width: 600,
  bgcolor: 'var(--accent-dark)',
  border: '2px solid #000',
  boxShadow: 50,
  p: 4,
};

export default function Profilepage({}: Props) {
  const [profileData, setProfileData] = useState<profileDataType>(DefaultProfileData);
  // const [replays, setReplays] = useState<replayType[]>([]);
  const [tabValue, setTabValue] = React.useState(0);

  const [eloData, setEloData] = useState<eloDataType>(DefaultEloData);
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  const navigate = useNavigate();

  const { uid } = useParams();

  const navigateToFriends = () => {
      // 👇️ navigate to /contacts
      navigate('/friends');
  };
  
  const navigateToResetPassword = () => {
    // 👇️ navigate to /contacts
    navigate('/resetpassword');
};

  useEffect(() => {
    fetch(`/api/profile?uid=${uid}`)
    .then(resp => resp.json())
    .then(data => {
      const payload = data.payload;
      const eloPayload = data.payload.elo_history;
      setProfileData(payload);
      setEloData(eloPayload);
    })
  },[])

  useEffect(() => {
    // console.log(tabValue)
    console.log(profileData);
    // setReplays();
  }, [tabValue])

  const renderFilteredReplay = () => {
    switch (tabMap[tabValue]) {
      case "CLASSIC": {
        return profileData["match_history_filtered"].CLASSIC
      }
      case "TRIPLES": {
        return profileData["match_history_filtered"].TRIPLES
      }
      case "POTHOLES": {
        return profileData["match_history_filtered"].POTHOLES
      }
    }
  }
  

  const options = {
    tooltip: {
      trigger: 'axis'
    },
    legend: {
      data: ['Classic', 'Potholes', 'Triples'],
      textStyle: {
        color: '#E9CFEC'
      }
    },
    grid: {
      left: '3%',
      right: '4%',
      bottom: '3%',
      containLabel: true
    },
    toolbox: {
      feature: {
        saveAsImage: {}
      }
    },
    xAxis: {
      type: 'category',
      boundaryGap: false,
      nameTextStyle: {
        color: '#E9CFEC'
      }
      
    },
    yAxis: {
      type: 'value',
      nameTextStyle: {
        color: '#E9CFEC'
      }
    },
    series: [
      {
        name: 'Classic',
        type: 'line',
        data: eloData.CLASSIC
      },
      {
        name: 'Potholes',
        type: 'line',
        data: eloData.POTHOLES
      },
      {
        name: 'Triples',
        type: 'line',
        data: eloData.TRIPLES
      }
    ]
  };


  return (
    <Container>
      <YavalathButtonFixed/>
      <Container1>
        <Container>
          <ProfileCard name={profileData.username}/>
          <Button align-items="right" onClick={navigateToFriends}>Friends</Button>
          <Button align-items="right" onClick={navigateToResetPassword}>Reset Password</Button>
        </Container>
        <StatContainer>
          <StatTab value={tabValue} setValue={setTabValue} data={profileData}/>
          <Box margin="40px 30px">
            <Button background="var(--accent-purple)" width={400} onClick={handleOpen}>Elo History Chart</Button>
            <Modal
              open={open}
              onClose={handleClose}
            >
              <Box sx={modalStyle}>
                <Typography variant="h5" color="var(--accent-purple)">{"Elo History"}</Typography>
                <ReactEcharts
                  option={options}
                />
              </Box>
            </Modal>
          </Box>
        </StatContainer>
      </Container1>
      <TextContainer>
        <Typography variant="h3">{"Past Matches"}</Typography>
      </TextContainer>
      {renderFilteredReplay()?.map((replay: replayType) => {
        return (
          <Match onClick={() => {navigate(replay.link)}}>
            <Box display="flex" flexDirection="column"> 
              {replay.players.map((player) => (
                <Typography variant="h4">
                  {player.username} {player.elo_start}
                </Typography>
              ))}
            </Box>
          </Match> 
        )
      })}
    </Container>
  )
}