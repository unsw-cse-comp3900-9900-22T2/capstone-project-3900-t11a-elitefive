import * as React from 'react';
import Paper from '@mui/material/Paper';
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TablePagination from '@mui/material/TablePagination';
import TableRow from '@mui/material/TableRow';
import { useParams } from 'react-router-dom';
import { useGameState } from '../global/GlobalGameState';
import { useEffect } from 'react';





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

export default function StickyHeadTable() {
  const [matchData, setMatchData] = React.useState<matchDataType>(DefaultMatchData);

  const{ matchid } = useParams();
  const { setWinner } = useGameState();

  useEffect(() => {
    fetch(`/api/replay?matchid=${matchid}`)
    .then(resp => resp.json())
    .then(data => {
      setMatchData(data);
    })
  },[])

  const row = matchData.move_seq.map(function (item, i) {
    const cell = item.map(function (element, j) {
      return (
        <TableRow hover >
          <TableCell
          key={j}
          style={{ minWidth: 100 }}
          align={'center'}
          >
            {element}
          </TableCell>
        </TableRow>
        );
    });
    return (
      <TableCell 
      key={i} 
      style={{width: 100}} 
      padding={'none'}
      > 
      {cell} 
      </TableCell>
    );
  })


  return (
    <Paper sx={{ width: '100%', overflow: 'hidden' }}>
      <TableContainer sx={{ maxHeight: 440 }}>
        <Table stickyHeader aria-label="sticky table">
          <TableHead>
            <TableRow>
                {matchData.players.map((x, index) => (
                  <TableCell
                  key={index}
                  style={{ minWidth: 100}}
                  align={'center'}
                  >
                    {x.username}
                  </TableCell>
                ))}
            </TableRow>
          </TableHead>
          <TableBody style={{verticalAlign:'top'}}>
            {row}
          </TableBody>
        </Table>
      </TableContainer>
    </Paper>
  );
}
