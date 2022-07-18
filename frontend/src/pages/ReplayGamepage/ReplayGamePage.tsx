import React, { useEffect, useRef, useState } from 'react'
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
  const{ matchid } = useParams();
  const { setWinner } = useGameState();



  useEffect(() => {
    fetch(`/api/replay?matchid=${matchid}`)
    .then(resp => resp.json())
    .then(data => {
      setGamemode(data.gamemode);
      setMatchData(data);
    })
  },[])

  return (
         <Container>
           <YavalathButtonFixed/>
           <Container2>
              <ReplayBoard/>
              <Sidebar/>
           </Container2>
         </Container>
  )
}

export function Sidebar({}: Props) {
  const [matchData, setMatchData] = useState<matchDataType>(DefaultMatchData);

  const{ matchid } = useParams();
  const { setWinner } = useGameState();


  useEffect(() => {
    fetch(`/api/replay?matchid=${matchid}`)
    .then(resp => resp.json())
    .then(data => {
      setMatchData(data);
    })
  },[])

  const navigate = useNavigate();

  const navigateToDashboard = () => {
    // 👇️ navigate to /contacts
    navigate('/dashboard');
  };

  const moveArray = matchData.moves.match(/.{1,2}/g);

  const stateArray = {move: ['']};

  const state = {index:0};


  const moveNext = () => {
    if (moveArray==null) return;
    setState({index: (state.index +1) % moveArray.length})
    let {move, moveArray[state.index]} = stateArray;
    move.push(moveArray[state.index]);

  }

  const movePrev = () => {
    if (moveArray==null || state.index==0) return;
    setState({index: (state.index +1) % moveArray.length})
    let {move, moveArray[state.index]} = stateArray;
    move.push(moveArray[state.index]);

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