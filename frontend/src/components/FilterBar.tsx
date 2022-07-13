import React, { SetStateAction, useEffect, useRef, useState } from 'react'
import styled from 'styled-components';
import Dropdown from './Dropdown';
import StyledInput from './StyledInput';

type Props = {
  filter: string | undefined;
  setFilter: React.Dispatch<SetStateAction<string|undefined>>;
  secondaryFilter: string | undefined;
  setSecondaryFilter: React.Dispatch<SetStateAction<string|undefined>>;
}

const Container = styled.div`
  margin-top: 10px;
  position: absolute;
  display: flex;
  gap: 50px;
`;

export const filterSelections = [
  "none",
  "elo",
  "mode",
  "type"
]

export const eloSelections = [
  "> 1000",
  "<= 1000"
]

export const modeSelections = [
  "Ranked",
  "Casual",
]
export const typeSelections = [
  "Classic",
  "Triples",
  "Potholes"
]

export default function FilterBar({filter, setFilter, secondaryFilter, setSecondaryFilter}: Props) {
  
  const renderDropDowns = () => {
    switch(filter) {
      case "elo":
        return (
          <Dropdown 
            selected={secondaryFilter}
            selections={eloSelections} 
            setSelected={(selection: string) => {
              setSecondaryFilter(selection);
            }}
          />
        )
      case "mode":
        return (
          <Dropdown 
            selected={secondaryFilter}
            selections={modeSelections} 
            setSelected={(selection: string) => {
              setSecondaryFilter(selection);
            }}
          />
        )
      case "type":
        return (
          <Dropdown 
            selected={secondaryFilter}
            selections={typeSelections} 
            setSelected={(selection: string) => {
              setSecondaryFilter(selection);
            }}
          />
        )
    }
  }

  return (
    <Container>
      <StyledInput size="small"/>
      <Dropdown 
        selected={filter}
        selections={filterSelections} 
        setSelected={(selection: string) => {
          if(selection == "none") {
            setFilter(undefined);
            return;
          }
          setFilter(selection);
          // reset secondary filters when you change filter mode
          setSecondaryFilter(undefined);
        }}
      />
      {renderDropDowns()}
    </Container>
  )
}