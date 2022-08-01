import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import { Box, Typography } from '@mui/material';
import ReplayPreview from '../components/ReplayPreview';
import FilterBar from '../components/FilterBar';
import SnapshotPopup from '../components/SnapshotPopup';
import YavalathButton from '../components/YavalathButton';

import * as API from '../api/rest';
import { useNavigate } from 'react-router-dom';
import Button from '../components/ReusableButton';

type Props = {}

export type replayDataType = {
  match_id: number;
  mode: "Ranked" | "Casual";
  gamemode: "CLASSIC" | "TRIPLES" | "POTHOLES"
  // result: "win" | "loss" | "draw";
  date: string;
  img: string;
  players: any[]; // todo
  "svg_data": string;
}

// const mockData: replayDataType[] = [
//   {
//     mode: "Ranked",
//     gamemode: "CLASSIC",
//     result: "win",
//     date: "15/3/2022",
//     img: "",
//     players: [], // todo
//   },
//   {
//     mode: "Casual",
//     gamemode: "TRIPLES",
//     result: "loss",
//     date: "15/3/2022",
//     img: "",
//     players: [], // todo
//   },
//   {
//     mode: "Casual",
//     gamemode: "POTHOLES",
//     result: "loss",
//     date: "15/3/2022",
//     img: "",
//     players: [], // todo
//   },
//   {
//     mode: "Casual",
//     gamemode: "POTHOLES",
//     result: "loss",
//     date: "15/3/2022",
//     img: "",
//     players: [], // todo
//   }
// ]

const MainContainer = styled.div`
  width: 90vw;
  margin: 40px 70px;

  display: flex;
  flex-direction: column;
`;

const ReplaysContainer = styled.div`
  display: flex;
  flex-wrap: wrap;
`;

const SideBarContainer = styled.div`
  background: var(--accent-dark);
  width: 20vw;
  height: 500px;
  margin: 0 50px;
  padding: 50px;
  display:flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 10px;

  position: sticky;
  top: 200px;
`;

const Highlight = styled.div<{isSelected: boolean}>`
  border: ${props => props.isSelected ? 
    '5px solid var(--accent-blue)' :
    '5px solid var(--accent-dark)'
  };
  margin: 50px; /* margin has to be placed in this level otherwise it wont work */
`;

// // helper fnc
// const filterData = (data: replayDataType[], filterType:string|undefined, secondaryFilter:string|undefined) => {
//   if(!data || !filterType || !secondaryFilter) {
//     return data;
//   }
//   switch(filterType) {
//     case "mode": {
//       return data.filter(d => d.mode == secondaryFilter)
//     }
//     case "type": {
//       return data.filter(d => d.gamemode == secondaryFilter)
//     }
//     // case "elo": {
//     //   // return eloFilter(data, secondaryFilter);
//     //   return data;
//     // }
//     default: {
//       return data;
//     }
//   }
// }


// const eloFilter = (data:replayDataType[], secondaryFilter:string) => {
//   switch(secondaryFilter) {
//     case "> 1000": {
//       return data.filter(d => d.elo > 1000);
//     }
//     case "<= 1000": {
//       return data.filter(d => d.elo <= 1000);
//     }
//   }
      
// }

export default function ReplaySearchpage({}: Props) {
  const [replays, setReplays] = useState<replayDataType[]|undefined>();
  const [filter, setFilter] = useState<string|undefined>();
  const [secondaryFilter, setSecondaryFilter] = useState<string|undefined>();
  const [isOpen, setIsOpen] = useState<boolean>(false);
  const [sideBarData, setSideBarData] = useState<replayDataType|undefined>();
  const [clickedRef, setClickedRef] = useState<number>(-1);

  const navigate = useNavigate();

  // on mount
  useEffect(() => {
    fetchAllMatches();
  },[])

  // on filter change if choose none as filter mode
  useEffect(() => {
    if(filter === undefined) {
      fetchAllMatches();
    }
  },[filter])

  // when secondary filter value changes
  useEffect(() => {
    fetchFiltered()
  }, [filter, secondaryFilter])
  
  const fetchAllMatches = async () => {
    const result = await API.getAllReplays()
    if(!result) return;
    const { all_matches: matches } = result;
    setReplays(matches);
    console.log(matches);
  }

  const fetchFiltered = async () => {
    switch(filter) {
      case "type": {
        await fetchTypeFilter();
        break;
      }
      case "game": {
        await fetchGameFilter();
        break;
      }
      default: {
        break;
      }
    }
  }

  const fetchTypeFilter = async () => {
    switch(secondaryFilter) {
      case "Ranked": {
        const resp = await fetch('/api/search/all?filter=type&value=ranked');
        const data = await resp.json();
        const { all_matches } = data;
        setReplays(all_matches);
        break;
      }
      case "Casual": {
        const resp = await fetch('/api/search/all?filter=type&value=casual');
        const data = await resp.json();
        const { all_matches } = data;
        setReplays(all_matches);
        break;
      }
    }
  }

  const fetchGameFilter = async () => {
    switch(secondaryFilter) {
      case "Classic": {
        const resp = await fetch('/api/search/all?filter=game&value=classic');
        const data = await resp.json();
        const { all_matches } = data;
        setReplays(all_matches);
        break;
      }
      case "Triples": {
        const resp = await fetch('/api/search/all?filter=game&value=triples');
        const data = await resp.json();
        const { all_matches } = data;
        setReplays(all_matches);
        break;
      }
      case "Potholes" : {
        const resp = await fetch('/api/search/all?filter=game&value=potholes');
        const data = await resp.json();
        const { all_matches } = data;
        setReplays(all_matches);
        break;
      }
    }
  }
  
  return (
    <>
    <YavalathButton/>
    <Box
      display="flex"
      width="100vw"
      minHeight="100vh"
    >
      <SnapshotPopup 
        open={isOpen} 
        handleClose={() => { setIsOpen(false) }}
        setReplayData={setReplays}
      />
      <MainContainer>
        <Box margin="30px 50px">
          <Typography variant="h4">Replays</Typography>
          <FilterBar
            filter={filter}
            secondaryFilter={secondaryFilter}
            setFilter={setFilter}
            setSecondaryFilter={setSecondaryFilter}
            setIsOpen={setIsOpen}
            setReplays={setReplays}
          />
        </Box>
        <ReplaysContainer>
          {/* {replays && filterData(replays as replayDataType[], filter, secondaryFilter)?.map((data: replayDataType) => ( */}
          {replays && replays.map((data: replayDataType) => (
              <Highlight isSelected={clickedRef == data.match_id}>
                <ReplayPreview {...data} setSideBarData={() => {
                  setSideBarData({...data})
                  setClickedRef(data.match_id);
                  console.log(data)
                }}/>
              </Highlight>
            ))
          }
        </ReplaysContainer>
      </MainContainer>
        <SideBarContainer>
          {sideBarData && (
            <>
            {/* <Typography variant="h5">{sideBarData.match_id}</Typography> */}
            <Typography variant="h4">{sideBarData.gamemode}</Typography>
            <Typography variant="h5">{sideBarData.mode}</Typography>
            {sideBarData.players.map((_, index) => {
              return (
                <>
                  <Typography variant="h5">
                    {`${sideBarData.players[index].username} ${index < 3 ? "vs":""}`}
                  </Typography>
                </>
              )
            })}
            {sideBarData.players.map((_, index) => {
              return (
                <>
                  <Typography variant="h5">
                    {`${sideBarData.players[index].username} : ${sideBarData.players[index].outcome}`}
                  </Typography>
                </>
              )
            })}
            <Button onClick={() => { navigate(`/replay/${sideBarData.match_id}`)}} background="var(--accent-purple)">
              Watch Replay
            </Button>
            </>
          )}
        </SideBarContainer>
    </Box>
    </>
  )
}