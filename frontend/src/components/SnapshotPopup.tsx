import React, { SetStateAction, useState } from 'react'
import styled from 'styled-components';
import { Modal } from '@mui/material';
import SnapshotBoard from './SnapshotBoard';
import Button from './ReusableButton';
import * as API from '../api/rest';
import { replayDataType } from '../pages/ReplaySearchpage';


type Props = {
  open: boolean;
  handleClose: () => void;
  setReplayData: React.Dispatch<SetStateAction<replayDataType[]|undefined>>
}

const Container = styled.div`
  width: 800px;
  height: 550px;
  background: var(--accent-dark);
  color: white;

  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);

  display:flex;
`;

const Left = styled.div`
  width: 80%;
`;

const Right = styled.div`
  background: var(--accent-darker);
  display:flex;
  flex-direction:column;
  padding: 2%;
  padding-top: 20%;
  
`


export default function SnapshotPopup({open, handleClose, setReplayData}: Props) {

  const [replayString, setReplayString] = useState<string[]>([]);

  const handleSnapshotSearch = async () => {
    const RString = replayString.join('');
    const results = await API.querySnapShot(RString);
    
    if(results.snapshot_matches == null) {
      setReplayData(undefined);
    } else {
      setReplayData(results.snapshot_matches);
    }
    handleClose();
  }

  return (
    <Modal open={open} onClose={handleClose}>
      <Container>
        <Left>
          <SnapshotBoard replayString={replayString} setReplayString={setReplayString}/>
        </Left>
        <Right>
          <Button onClick={handleSnapshotSearch}>
            Confirm
          </Button>
        </Right>
      </Container>
    </Modal>
  )
}