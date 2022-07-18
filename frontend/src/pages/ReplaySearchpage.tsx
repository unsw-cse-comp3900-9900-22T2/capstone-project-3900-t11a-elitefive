import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import { Box, Typography } from '@mui/material';
import ReplayPreview from '../components/ReplayPreview';
import FilterBar from '../components/FilterBar';
import SnapshotPopup from '../components/SnapshotPopup';
import YavalathButton from '../components/YavalathButton';

import * as API from '../api/rest';

type Props = {}

export type replayDataType = {
  match_id: number;
  mode: "Ranked" | "Casual";
  gamemode: "CLASSIC" | "TRIPLES" | "POTHOLES"
  // result: "win" | "loss" | "draw";
  date: string;
  img: string;
  players: any[]; // todo
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
  margin: 120px 80px;
  display:flex;
  flex-direction: column;
`;

// helper fnc
const filterData = (data: replayDataType[], filterType:string|undefined, secondaryFilter:string|undefined) => {
  if(!data || !filterType || !secondaryFilter) {
    return data;
  }
  switch(filterType) {
    case "mode": {
      return data.filter(d => d.mode == secondaryFilter)
    }
    case "type": {
      return data.filter(d => d.gamemode == secondaryFilter)
    }
    case "elo": {
      // return eloFilter(data, secondaryFilter);
      return data;
    }
    default: {
      return data;
    }
  }
}


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
  
  const fetchAllMatches = async () => {
    const result = await API.getAllReplays()
    if(!result) return;
    const { all_matches: matches } = result;
    setReplays(matches);
    console.log(matches);
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
          />
        </Box>
        <ReplaysContainer>
          {replays && filterData(replays as replayDataType[], filter, secondaryFilter)?.map((data: replayDataType) => (
              <ReplayPreview {...data} setSideBarData={() => {
                setSideBarData({...data})
              }}/>
            ))
          }
        </ReplaysContainer>
      </MainContainer>
      {sideBarData && (
        <SideBarContainer>
          <Typography>{sideBarData.match_id}</Typography>
          <Typography>{sideBarData.gamemode}</Typography>
          <Typography>{sideBarData.mode}</Typography>
          <Typography>{}</Typography>
        </SideBarContainer>
      )}
    </Box>
    </>
  )
}