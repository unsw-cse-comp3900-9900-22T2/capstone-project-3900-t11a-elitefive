import React from 'react'
import styled from 'styled-components';
import { Modal } from '@mui/material';
import SnapshotBoard from './SnapshotBoard';
import Button from './ReusableButton';


type Props = {
  open: boolean;
  handleClose: () => void;
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


export default function SnapshotPopup({open, handleClose}: Props) {
  return (
    <Modal open={open} onClose={handleClose}>
      <Container>
        <Left>
          <SnapshotBoard/>
        </Left>
        <Right>
          <Button>
            Confirm
          </Button>
        </Right>
      </Container>
    </Modal>
  )
}