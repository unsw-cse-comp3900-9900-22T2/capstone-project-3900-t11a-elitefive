import React, { SetStateAction, useEffect, useRef, useState } from 'react'
import styled from 'styled-components';

// local
import { GSProvider, useGameState } from '../../global/GlobalGameState';
import { WSProvider } from '../../global/GlobalWS';
import Board, { ReplayBoard } from '../../components/Board';
import YavalathButton, { YavalathButtonFixed } from '../../components/YavalathButton';


import BasicCard, {Card2, Card3, ReplayPlayerCard} from '../../components/ReusableCard';
import Typography from '@mui/material/Typography';
import Button, {Button2} from '../../components/ReusableButton';

import StickyHeadTable from '../../components/Tables';
import { useNavigate, useParams } from 'react-router-dom';
import { useAuth } from '../../global/GlobalAuth';
import { NumberLiteralType, setConstantValue } from 'typescript';
import { match } from 'assert';
import { Box } from '@mui/material';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;
  

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const Container2 = styled.div`
  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const ContainerArrow = styled.div`
  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export type matchDataType = {
  gamemode: string;
  holes: string;
  match_id: number;
  move_seq: Array<Array<string>>;
  moves: string;
  nplayers: number;
  players: Array<playersDataType>;
}

type playersDataType = {
  elo_end: number;
  elo_start: number;
  outcome: string;
  uid: number;
  username: string;
}

const DefaultMatchData = {
    "gamemode": "",
    "holes": "",
    "match_id": 0,
    "move_seq": [
        [
            ""
        ],
        [
            ""
        ]
    ],
    "moves": "",
    "nplayers": 0,
    "players": [
        {
            "elo_end": 0,
            "elo_start": 0,
            "outcome": "",
            "uid": 0,
            "username": ""
        },
        {
            "elo_end": 0,
            "elo_start": 0,
            "outcome": "",
            "uid": 0,
            "username": ""
        }
    ],
}

const DefaultGamemode = {
  "gamemode" : "",
}

export type gamemodeDataType = {
  gamemode: string;
}

export default function ReplayGamepage({}: Props) {

  const [matchData, setMatchData] = useState<matchDataType>(DefaultMatchData);
  const [gamemode, setGamemode] = useState<gamemodeDataType>(DefaultGamemode);
  const [nplayers, setPlayerCount] = useState<number>(2);
  const{ matchid } = useParams();
  const { setWinner } = useGameState();

  const [replayStringArr, setReplayStringArr] = useState<string[]>([])
  const [currIndex, setCurrIndex] = useState(0);



  useEffect(() => {
    fetch(`/api/replay?matchid=${matchid}`)
    .then(resp => resp.json())
    .then(data => {
      setGamemode(data.gamemode);
      setMatchData(data);
      // console.log(data);
      // console.log(data.nplayers);
      const nplayers = data.nplayers;
      setPlayerCount(nplayers);
      console.log("Is set to:")
      console.log(nplayers);

    })
  },[])

  return (
         <Container>
           <YavalathButtonFixed/>
           <Container2>
            {
            (matchData === DefaultMatchData) ? 
            (
              <Box display="flex" alignItems="center" height="100vh">
                <Typography variant="h3">Replay is not found</Typography> 
              </Box>
            ):  
            (
              <>
                <ReplayBoard 
                  replayStringArr={replayStringArr}
                  numPlayers={nplayers}
                />
                <Sidebar
                  matchData={matchData}
                  currIndex={currIndex}
                  replayStringArr={replayStringArr}
                  setCurrIndex={setCurrIndex}
                  setReplayStringArr={setReplayStringArr}
                />
              </>
            )
            }
           </Container2>
         </Container>
  )
}

type SideBarProps = {
  matchData: matchDataType;
  currIndex: number;
  replayStringArr: string[];
  setCurrIndex: React.Dispatch<SetStateAction<number>>
  setReplayStringArr: React.Dispatch<SetStateAction<string[]>>
}

export function Sidebar({ matchData, currIndex, replayStringArr, setCurrIndex, setReplayStringArr}: SideBarProps) {

  const navigate = useNavigate();

  const navigateToDashboard = () => {
    // 👇️ navigate to /contacts
    navigate('/dashboard');
  };

  const splitReplayString = matchData.moves.match(/.{2}/g);


  const moveNext = () => {
    if(!splitReplayString) return;
    setCurrIndex(prev=>prev+1)
    setReplayStringArr(splitReplayString.slice(0, currIndex+1))
    console.log(replayStringArr);
  }

  const movePrev = () => {
    if (!splitReplayString) return;
    if(currIndex == 0) return;
    setCurrIndex(prev=>prev-1)
    setReplayStringArr(splitReplayString.slice(0, currIndex-1))
    console.log(replayStringArr);
  }

  return (
    <Container>
      <Typography sx={{ fontSize: 80 }} color="white" gutterBottom>
          Replay
        </Typography>
      <ReplayPlayerCard name1={matchData.players[0].username} name2={matchData.players[1].username}/>
      {/* for loop to go through names also add elo*/}
      <Card2>
        <StickyHeadTable/>
      </Card2>
      <ContainerArrow>
        <Button onClick={movePrev}>Back</Button>
        <Button onClick={moveNext}>Next</Button>
      </ContainerArrow>
      <Button 
        width={350} 
        height={40}
        onClick={navigateToDashboard}
      >
        Exit
      </Button>
    </Container>
  )
}