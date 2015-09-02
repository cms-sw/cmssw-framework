#!/usr/bin/perl

use warnings;
use strict;

use DBI;
use DBD::Oracle qw(:ora_types);


die "Usage:  occupancy.pl min_n_lo min_n_hi\n" unless ($#ARGV == 1);

my ($min_n_lo, $min_n_hi) = @ARGV;

my $dbh = my_connect(db => 'ecalh4db',
		  user => 'read01',
		  pass => 'oraread01',
		  db_opts => { RaiseError => 1 }
		  );

my $sql = qq[ SELECT riov.run_num run, miov.subrun_num subrun, rdat.id1 SM, cv.id2 crystal, 
                     occ.events_over_low_threshold n_lo, occ.events_over_high_threshold n_hi
                FROM run_iov riov
                JOIN run_tag rtag ON rtag.tag_id = riov.tag_id
                JOIN location_def ldef ON ldef.def_id = rtag.location_id
                JOIN (SELECT iov_id, cv.id1 FROM run_dat rdat 
                        JOIN channelview cv ON cv.logic_id = rdat.logic_id AND cv.name = cv.maps_to) rdat
                     ON rdat.iov_id = riov.iov_id
                JOIN mon_run_iov miov ON miov.run_iov_id = riov.iov_id
	        JOIN mon_occupancy_dat occ ON occ.iov_id = miov.iov_id
                JOIN channelview cv ON cv.logic_id = occ.logic_id AND cv.name = cv.maps_to
               WHERE ldef.location = 'H4B'
                 AND occ.events_over_low_threshold >= ?
	         AND occ.events_over_high_threshold >= ?
            ORDER BY run, SM, crystal];

my $sth = $dbh->prepare_cached($sql);

$sth->execute($min_n_lo, $min_n_hi);

print join("\t", @{$sth->{NAME}}), "\n";
while (my @row = $sth->fetchrow()) {
    print join("\t", @row), "\n";
}



sub my_connect {
  my %args = @_;
  my $db = $args{db};
  my $user = $args{user};
  my $pass = $args{pass};
  my $port = $args{port} || 1521;
  my $db_opts = $args{db_opts};

  # env
  $ENV{"ORACLE_HOME"} = '/afs/cern.ch/project/oracle/@sys/10103';
  $ENV{"TNS_ADMIN"} = '/afs/cern.ch/project/oracle/admin';
  $ENV{"NLS_LANG"} = "AMERICAN";

  # here we use the TNS_ADMIN file and $db is the SID
  my $dsn;
  if ($db) {
    $dsn = "DBI:Oracle:$db";
  } else {
    die "Oracle needs to have database defined on connection!\n";
  }

  my $dbh = DBI->connect($dsn, $user, $pass, $db_opts)
    or die "Database connection failed, $DBI::errstr";

  $dbh->do(qq[ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI:SS']);

  return $dbh;
}
