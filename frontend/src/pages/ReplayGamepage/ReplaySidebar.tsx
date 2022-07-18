import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import BasicCard, {Card2, Card3, ReplayPlayerCard} from '../../components/ReusableCard';
import Typography from '@mui/material/Typography';
import Button, {Button2} from '../../components/ReusableButton';
import { useGameState } from '../../global/GlobalGameState';
import StickyHeadTable from '../../components/Tables';
import { useNavigate, useParams } from 'react-router-dom';
import { useAuth } from '../../global/GlobalAuth';
import { NumberLiteralType } from 'typescript';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;

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


export default function Sidebar({}: Props) {
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

  return (
    <Container>
      <Typography sx={{ fontSize: 80 }} color="white" gutterBottom>
          Replay
        </Typography>
      <ReplayPlayerCard name1={matchData.players[0].username} name2={matchData.players[1].username}/>
      <Card2>
        <StickyHeadTable/>
      </Card2>
      <ContainerArrow>
        <Button>Back</Button>
        <Button>Next</Button>
      </ContainerArrow>
      <Button 
        width={350} 
        height={40}
        background="red"
        onClick={() => { setWinner("COMPUTER") }}
      >
        Retire
      </Button>
    </Container>
  )
}