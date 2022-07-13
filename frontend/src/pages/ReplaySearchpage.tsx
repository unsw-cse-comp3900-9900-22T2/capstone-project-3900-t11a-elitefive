import React, { useState } from 'react'
import styled from 'styled-components';
import { Box, Typography } from '@mui/material';
import ReplayPreview from '../components/ReplayPreview';
import FilterBar from '../components/FilterBar';

type Props = {}

type replayDataType = {
  mode: "Ranked" | "Casual";
  type: "Classic" | "Triples" | "Potholes"
  result: "win" | "loss" | "draw";
  date: string;
  img: string;
  players: string[]; // todo
  elo: number;
}

const mockData = [
  {
    mode: "Ranked",
    type: "Classic",
    result: "win",
    date: "15/3/2022",
    img: "",
    players: [], // todo
    elo: 1000
  },
  {
    mode: "Casual",
    type: "Triples",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [], // todo
    elo: 1000
  },
  {
    mode: "Casual",
    type: "Potholes",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [], // todo
    elo: 1000
  },
  {
    mode: "Casual",
    type: "Potholes",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [], // todo
    elo: 2000
  }
]

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
      return data.filter(d => d.type == secondaryFilter)
    }
    case "elo": {
      return eloFilter(data, secondaryFilter);
    }
    default: {
      return data;
    }
  }
}


const eloFilter = (data:replayDataType[], secondaryFilter:string) => {
  switch(secondaryFilter) {
    case "> 1000": {
      return data.filter(d => d.elo > 1000);
    }
    case "<= 1000": {
      return data.filter(d => d.elo <= 1000);
    }
  }
      
}

export default function ReplaySearchpage({}: Props) {
  const [filter, setFilter] = useState<string|undefined>();
  const [secondaryFilter, setSecondaryFilter] = useState<string|undefined>();
  
  return (
    <Box
      display="flex"
      width="100vw"
      minHeight="100vh"
    >
      <MainContainer>
        <Box margin="30px 50px">
          <Typography variant="h4">Replays</Typography>
          <FilterBar
            filter={filter}
            secondaryFilter={secondaryFilter}
            setFilter={setFilter}
            setSecondaryFilter={setSecondaryFilter}
          />
        </Box>
        <ReplaysContainer>
          {filterData(mockData as replayDataType[], filter, secondaryFilter)?.map((data) => (
            <ReplayPreview {...data}/>
          ))}
        </ReplaysContainer>
      </MainContainer>
      <SideBarContainer>
        bye
      </SideBarContainer>
    </Box>
  )
}