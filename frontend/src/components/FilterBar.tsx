import React, { SetStateAction, useEffect, useRef, useState, useCallback } from 'react'
import styled from 'styled-components';
import Dropdown from './Dropdown';

import StyledInput from './StyledInput';
import Button from './ReusableButton';
import { replayDataType } from '../pages/ReplaySearchpage';
import _debounce from 'lodash/debounce';

type Props = {
  filter: string | undefined;
  setFilter: React.Dispatch<SetStateAction<string|undefined>>;
  secondaryFilter: string | undefined;
  setSecondaryFilter: React.Dispatch<SetStateAction<string|undefined>>;
  setIsOpen: React.Dispatch<SetStateAction<boolean>>;
  setReplays: React.Dispatch<SetStateAction<replayDataType[]|undefined>>;
}

const Container = styled.div`
  margin-top: 10px;
  position: absolute;
  display: flex;
  gap: 50px;
`;

export const filterSelections = [
  "none",
  // "elo",
  "player",
  "mode",
  "type",
  "snapshot"
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

export default function FilterBar({filter, setFilter, secondaryFilter, setSecondaryFilter, setIsOpen, setReplays}: Props) {

  const [input, setInput] = useState<string>('');

  const clearReplay = () => {
    return new Promise((resolve, reject) => {
      setReplays(undefined);
      resolve(true );
    })
  }

  const handleDebounceFn = async (querystr: string) => {
    const resp = await fetch(`/api/search/all?filter=player&value=${querystr}`);
    const data = await resp.json();
    const { all_matches } = data;
    setReplays(all_matches);
  }

  const debounceFn = useCallback(_debounce(handleDebounceFn, 1000),[]);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setInput(e.target.value);
    debounceFn(e.target.value);
  }

  const renderDropDowns = () => {
    switch(filter) {
      // case "elo":
      //   return (
      //     <Dropdown 
      //       selected={secondaryFilter}
      //       selections={eloSelections} 
      //       setSelected={(selection: string) => {
      //         setSecondaryFilter(selection);
      //       }}
      //     />
      //   )
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
      case "snapshot":
        return (
          <Button height={33} onClick={() => { setIsOpen(true) }}>
            Snapshot
          </Button>
        )
    }
  }

  return (
    <Container>
      <StyledInput value={input} size="small" disabled={filter!=="player"} onChange={handleChange}/>
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